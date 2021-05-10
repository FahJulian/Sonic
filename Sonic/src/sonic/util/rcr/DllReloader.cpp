#include "DllReloader.h"

#include <Windows.h>
#include <ImageHlp.h>

#include "sonic/debug/Log.h"

#include "sonic/util/Map.h"
#include "sonic/util/DynamicArray.h"
#include "sonic/util/platform/FileUtils.h"

namespace sonic
{
	namespace
	{
		struct Dll
		{
			struct Symbol
			{
				String nativeName;
				uintptr_t* pointer;
			};

			String name;
			HINSTANCE win32Handle;
			Map<String, Symbol> functions;
		};

		constexpr const char* COMPILER_COMMAND = "%COMPILER_PATH "
			"/c /ZI /JMC /nologo /W3 /WX- /diagnostics:column /sdl /Gm- /EHsc /MDd /GS /fp:precise "
			"/permissive- /Zc:wchar_t /Zc:forScope /Zc:inline /std:c++17 /Gd /TP /FC /errorReport:queue "
			"/D SN_DLL_EXPORT /D SN_DLL_EXPORT_DEBUG /D _CONSOLE /D _WINDLL /D _UNICODE /D UNICODE ";

		constexpr const char* LINKER_COMMAND = "%LINKER_PATH "
			"/ERRORREPORT:QUEUE /INCREMENTAL /NOLOGO /MANIFEST /MANIFESTUAC:\"level='asInvoker' uiAccess='false'\" "
			"/manifest:embed /DEBUG:NONE /SUBSYSTEM:CONSOLE /TLBID:1 /DYNAMICBASE /NXCOMPAT /MACHINE:X64 /DLL "
			"kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ";

		Map<String, Dll> sDlls;
		DllReloader::Settings sSettings;
		bool sInitialized = false;

		String _generateCompilerCommand(const String& dllName, const DynamicArray<String>& sourceFiles, const DynamicArray<String>& prepocessorDefinitions)
		{
			String command = COMPILER_COMMAND;
			command.replaceFirst("%COMPILER_PATH", sSettings.compilerExePath);
			command += sSettings.configuration == DllReloader::Configuration::DEBUG ? "/Od " : "/02 ";
			command += "/Fo\"" + sSettings.tmpDir + dllName + "\\\\\" ";

			for (auto& preprocessorDefinition : prepocessorDefinitions)
				command += "/D " + preprocessorDefinition + " ";

			for (auto& sourceFile : sourceFiles)
				command += sourceFile + " ";

			for (auto& includeDir : sSettings.includeDirs)
				command += "/I\"" + includeDir + "\" ";

			return command;
		}

		String _generateLinkerCommand(const String& dllName, const DynamicArray<String> objFiles)
		{
			String command = LINKER_COMMAND;
			command.replaceFirst("%LINKER_PATH", sSettings.linkerExePath);
			command += "/IMPLIB:\"" + sSettings.tmpDir + dllName + "\\" + dllName + ".lib\" ";
			command += "/OUT:\"" + sSettings.tmpDir + dllName + "\\" + dllName + ".dll\" ";

			for (auto& libDir : sSettings.libDirs)
				command += "/LIBPATH:\"" + libDir + "\" ";

			for (auto& objFile : objFiles)
				command += sSettings.tmpDir + dllName + "\\" + objFile + " ";

			return command;
		}

		bool _compilationSuccessfull(const Dll& dll)
		{
			return (platform::fileExists(sSettings.tmpDir + dll.name + "\\" + dll.name + ".dll") &&
				platform::fileExists(sSettings.tmpDir + dll.name + "\\" + dll.name + ".lib"));
		}

		Map<String, String> _extractSymbols(const String& dllPath)
		{
			Map<String, String> symbols;
			LOADED_IMAGE loadedImage;
			DWORD directorySize;

			if (MapAndLoad(dllPath.getData(), NULL, &loadedImage, NULL, TRUE))
			{
				IMAGE_EXPORT_DIRECTORY* imageExportDirectory = reinterpret_cast<IMAGE_EXPORT_DIRECTORY*>(
					ImageDirectoryEntryToData(loadedImage.MappedAddress, FALSE, IMAGE_DIRECTORY_ENTRY_EXPORT, &directorySize));

				if (imageExportDirectory)
				{
					DWORD* symbolsVirtualAddresses = reinterpret_cast<DWORD*>(ImageRvaToVa(loadedImage.FileHeader,
						loadedImage.MappedAddress, imageExportDirectory->AddressOfNames, NULL));

					constexpr size_t symbolNameBufferSize = 0xfff;
					char symbolNameBuffer[symbolNameBufferSize];

					for (size_t i = 0; i < imageExportDirectory->NumberOfNames; i++)
					{
						char* symbolAdress = reinterpret_cast<char*>(ImageRvaToVa(loadedImage.FileHeader, loadedImage.MappedAddress, symbolsVirtualAddresses[i], NULL));
						constexpr DWORD flags = UNDNAME_NO_ACCESS_SPECIFIERS | UNDNAME_NO_ALLOCATION_LANGUAGE | UNDNAME_NO_ALLOCATION_MODEL | UNDNAME_NO_SPECIAL_SYMS |
							UNDNAME_NO_RETURN_UDT_MODEL | UNDNAME_NO_MS_THISTYPE | UNDNAME_NO_MS_KEYWORDS;

						std::memset(symbolNameBuffer, 0, symbolNameBufferSize);
						UnDecorateSymbolName(symbolAdress, symbolNameBuffer, symbolNameBufferSize, flags);

						symbols.insert(String(symbolNameBuffer).replaceAll("(void)", "()").replaceAll(" >", ">"), symbolAdress);
					}
				}

				UnMapAndLoad(&loadedImage);
			}

			return symbols;
		}

		bool _reloadDll(Dll& dll, const String& path)
		{
			FreeLibrary(dll.win32Handle);
			dll.win32Handle = LoadLibraryA((path + dll.name + ".dll").getData());

			if (!dll.win32Handle)
				return false;

			Map<String, String> symbols = _extractSymbols(path + dll.name + ".dll");

			for (auto& [name, function] : dll.functions)
			{
				if (!symbols.containsKey(name))
				{
					Log::error("Error reloading dll: Function ", name, " not found");
					return false;
				}

				if (function.pointer)
					*function.pointer = reinterpret_cast<uintptr_t>(GetProcAddress(dll.win32Handle, symbols[name].getData()));
				symbols.remove(name);
			}

			// Not yet registered functions
			for (auto& [name, symbol] : symbols)
				dll.functions.insert(name, { symbol, nullptr });

			return true;
		}

		bool _swapFiles(const Dll& dll, const String& path)
		{
			if (String oldDllFileName = path + dll.name + ".dll";
				platform::fileExists(oldDllFileName))
			{
				platform::renameFile(oldDllFileName, path + dll.name + "-old.dll");
			}

			if (String oldLibFileName = path + dll.name + ".lib";
				platform::fileExists(oldLibFileName))
			{
				platform::renameFile(oldLibFileName, path + dll.name + "-old.lib");
			}

			return (platform::moveFile(sSettings.tmpDir + dll.name + '\\' + dll.name + ".dll", path + dll.name + ".dll") &&
				platform::moveFile(sSettings.tmpDir + dll.name + '\\' + dll.name + ".lib", path + dll.name + ".lib"));
		}

		bool _reloadOldDll(Dll& dll, const String& path)
		{
			return (platform::renameFile(path + dll.name + "-old.dll", path + dll.name + ".dll") &&
				platform::renameFile(path + dll.name + "-old.lib", path + dll.name + ".lib") &&
				_reloadDll(dll, path));
		}

		void _compileDll(const String& dllName, const DynamicArray<String>& cppFiles, const DynamicArray<String>& preprocessorDefinitions)
		{
			String compilerCommand = _generateCompilerCommand(dllName, cppFiles, preprocessorDefinitions);

			Log::info("Recompiling dll ", dllName, "...");

			std::system(compilerCommand.getData());
		}

		void _linkDll(const String& dllName, const DynamicArray<String>& objFiles)
		{
			String linkerCommand = _generateLinkerCommand(dllName, objFiles);

			Log::info("Linking dll ", dllName, "...");

			std::system(linkerCommand.getData());
		}

		void _deleteTemporaryFiles(const Dll& dll, const String& path, const DynamicArray<String>& objFiles)
		{
			String tmpDir = sSettings.tmpDir + dll.name + '\\';

			platform::deleteFile(path + dll.name + "-old.dll");
			platform::deleteFile(path + dll.name + "-old.lib");

			platform::deleteFile(tmpDir + dll.name + ".dll");
			platform::deleteFile(tmpDir + dll.name + ".lib");
			platform::deleteFile(tmpDir + dll.name + ".exp");
			platform::deleteFile(tmpDir + dll.name + ".ilk");

			for (auto& objFile : objFiles)
				platform::deleteFile(tmpDir + objFile);

			platform::deleteEmptyDirectoryRecursively(tmpDir);
		}

		bool _recompileAndReloadDll(Dll& dll, const String& path, DynamicArray<String> files, DynamicArray<String> preprocessorDefinitions)
		{
			String tmpDir = sSettings.tmpDir + dll.name + '\\';
			if (!platform::directoryExists(tmpDir))
				platform::createDirectoryRecursively(tmpDir);

			for (auto& file : files)
			{
				file.replaceAll('/', '\\');

				if (!file.endsWith(".cpp"))
				{
					Log::error("Can't recompile dll ", dll.name, ": invalid source file ", file, ". Only .cpp files are allowed.");
					return false;
				}
			}

			_compileDll(dll.name, files, preprocessorDefinitions);

			// Turn .cpp files into .obj files
			for (auto& file : files)
				file = file.subString(file.findLastOf('\\') + 1, file.findFirstOf(".cpp")) + ".obj";

			_linkDll(dll.name, files);

			if (!_compilationSuccessfull(dll))
			{
				Log::error("Could not recompile dll ", dll.name);
				return false;
			}

			Log::info("Done. Extracting symbols from dll ", dll.name, "...");

			if (!_swapFiles(dll, path) || !_reloadDll(dll, path))
			{
				Log::error("Could not extract symbols from dll ", dll.name);
				Log::error("trying to reload old dll...");

				if (!_reloadOldDll(dll, path))
				{
					Log::error("Failed.");

					_deleteTemporaryFiles(dll, path, files);
					return false;
				}

				Log::info("Done.");
			}

			Log::info("Done.");

			_deleteTemporaryFiles(dll, path, files);
			return true;
		}

	} // namespace

	void _putDirectoriesWithSpacesInQuotes(String& path)
	{
		size_t index = 0;
		while ((index = path.findFirstOf(' ', index)) != path.getSize())
		{
			path.replaceLast("\\", "\\\"", index);

			index = path.findFirstOf('\\', index);
			path.replace(index, index + 1, "\"\\");
		}
	}

	bool DllReloader::init(const Settings& settings)
	{
		sSettings = settings;
		sSettings.compilerExePath = sSettings.compilerExePath.trim().replaceAll('/', '\\');
		sSettings.linkerExePath = sSettings.linkerExePath.trim().replaceAll('/', '\\');

		if (!sSettings.compilerExePath.endsWithIgnoreCase("\\cl.exe"))
		{
			Log::error("[DllReloader] Invalid cl.exe path: ", sSettings.compilerExePath);
			return false;
		}

		if (!sSettings.linkerExePath.endsWithIgnoreCase("\\link.exe"))
		{
			Log::error("[DllReloader] Invalid link.exe path: ", sSettings.linkerExePath);
			return false;
		}

		_putDirectoriesWithSpacesInQuotes(sSettings.compilerExePath);
		_putDirectoriesWithSpacesInQuotes(sSettings.linkerExePath);

		if (!platform::fileExists(sSettings.tmpDir))
			platform::createDirectoryRecursively(sSettings.tmpDir);

		sInitialized = true;
		return true;
	}

	bool DllReloader::reloadDll(String dllPath, bool forceRecompile, std::initializer_list<String> sourceFiles,
		std::initializer_list<String> additionalPreprocessorDefinitions)
	{
		if (!sInitialized)
		{
			Log::warn("Can't recompile dll ", dllPath, ": DllReloader has not been initialized");
			return false;
		}

		dllPath.replaceAll('/', '\\');
		String dllDir = dllPath.subString(0, dllPath.findLastOf('\\') + 1);

		if (!sDlls.containsKey(dllPath))
			sDlls.insert(dllPath, { dllPath.subString(dllPath.findLastOf('\\') + 1, dllPath.findFirstOf('.')) });

		Dll& dll = sDlls[dllPath];

		if (forceRecompile)
		{
			if (!_recompileAndReloadDll(dll, dllDir, sourceFiles, additionalPreprocessorDefinitions))
				return false;
		}
		else
		{
			if (!_reloadDll(dll, dllDir))
			{
				Log::info("Reloading dll ", dllPath, " failed, trying to recompile...");

				if (!_recompileAndReloadDll(dll, dllDir, sourceFiles, additionalPreprocessorDefinitions))
					return false;
			}
		}

		return true;
	}

	void DllReloader::destroy()
	{
		for (auto& [name, dll] : sDlls)
		{
			Log::info("Freeing dll ", name, ".dll");

			FreeLibrary(dll.win32Handle);
		}
	}

	uintptr_t DllReloader::_registerFunction(const String& dllPath, uintptr_t* functionPointer, const String& functionName)
	{
		if (!sDlls.containsKey(dllPath))
		{
			Log::warn("Can't register function ", functionName, " for Dll ", dllPath, ": Dll is not registered for RCR");
			return 0;
		}

		Dll& dll = sDlls[dllPath];
		if (!dll.win32Handle)
		{
			Log::warn("Can't register function ", functionName, " for Dll ", dllPath, ": Dll has not been loaded.");
			return 0;
		}

		if (!dll.functions.containsKey(functionName))
		{
			Log::warn("Can't register function ", functionName, " for Dll ", dllPath, ": Dll does not cotain function");
			return 0;
		}

		dll.functions[dllPath].pointer = functionPointer;
		return reinterpret_cast<uintptr_t>(GetProcAddress(dll.win32Handle, dll.functions[functionName].nativeName.getData()));
	}

} // namespace sonic
