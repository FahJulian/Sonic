#pragma once

#if defined(SN_DLL_EXPORT)
	#error Exporting dlls should not use RCR to load another dll
#endif

#include "sonic/util/String.h"
#include "sonic/util/DynamicArray.h"

namespace sonic
{
	class DllReloader
	{
	public:
		template<typename R, typename... Args>
		using FunctionPointer = R(*)(Args...);

		template<typename R, typename T, typename... Args>
		using MemberFunctionPointer = R(T::*)(Args...);

		enum class Configuration : uint8_t
		{
			DEBUG,
			RELEASE
		};

		struct Settings
		{
			String compilerExePath;
			String linkerExePath;
			String tmpDir;
			DynamicArray<String> includeDirs;
			DynamicArray<String> libDirs;
			Configuration configuration = Configuration::DEBUG;
		};

		template<typename... Args>
		static String getTypeNames()
		{
			String string;
			_appendTypeNames<Args...>(string, false);
			return string;
		}

		template<>
		static String getTypeNames()
		{
			return { };
		}

		template<typename... Args>
		static String getTypeNames(bool isArgumentTypes)
		{
			String string;
			_appendTypeNames<Args...>(string, isArgumentTypes);
			return string;
		}

		template<>
		static sonic::String getTypeNames(bool isArgumentTypes)
		{
			return { };
		}

		template<typename R, typename... Args>
		static FunctionPointer<R, Args...> registerFunctionPointer(const String& dllPath, 
			FunctionPointer<R, Args...>* functionPointer, 
			const String& returnType, 
			const String& namespaceName, 
			const String& functionName, 
			const String& templateTypes, 
			const String& argumentTypes)
		{
			String fullFunctionName = returnType + ' ' + ((namespaceName != "::") ? namespaceName + functionName : functionName);

			if (!templateTypes.isEmpty())
				fullFunctionName += '<' + templateTypes + '>';

			fullFunctionName += '(' + argumentTypes + ')';

			uintptr_t function = _registerFunction(dllPath, reinterpret_cast<uintptr_t*>(functionPointer), fullFunctionName);
			return *reinterpret_cast<FunctionPointer<R, Args...>*>(&function);
		}

		template<typename R, typename T, typename... Args>
		static MemberFunctionPointer<R, T, Args...> registerMemberFunctionPointer(const String& dllPath, 
			MemberFunctionPointer<R, T, Args...>* memberFunctionPointer, 
			const String& returnType, 
			const String& memberFunctionName, 
			const String& templateTypes, 
			const String& argumentTypes)
		{
			String className = getTypeNames<T>(false);
			className = className.subString(className.findFirstOf(' ') + 1);	// remove the "class " or "struct " prefix

			String fullMemberFunctionName = returnType + ' ' + className + "::" + memberFunctionName;

			if (!templateTypes.isEmpty())
				fullMemberFunctionName += '<' + templateTypes + '>';

			fullMemberFunctionName += '(' + argumentTypes + ')';

			uintptr_t function = _registerFunction(dllPath, reinterpret_cast<uintptr_t*>(memberFunctionPointer), fullMemberFunctionName);
			return *reinterpret_cast<MemberFunctionPointer<R, T, Args...>*>(&function);
		}

		static bool init(const Settings& settings);

		static bool reloadDll(String dllPath, bool forceRecompile, std::initializer_list<String> sourceFiles, 
			std::initializer_list<String> additionalPreprocessorDefinitions = {});

		static void destroy();

	private:
		static uintptr_t _registerFunction(const String& dllPath, uintptr_t* functionPointer, const String& functionName);

		template<typename T>
		static void _appendTypeNames(String& string, bool isArgumentTypes, bool start = true)
		{
			if (!start)
				string.append(',');

			string.append(_getTypeName<T>(isArgumentTypes));
		}

		template<typename T, typename F, typename... Args>
		static void _appendTypeNames(String& string, bool isArugmentTypes, bool start = true)
		{
			_appendTypeNames<T>(string, isArugmentTypes, start);
			_appendTypeNames<F, Args...>(string, isArugmentTypes, false);
		}

		template<typename T>
		static constexpr String _getTypeName(bool ignoreModifiers)
		{
			if (std::is_pointer<T>::value)
			{
				String valueTypeName = _getTypeName<std::remove_pointer<T>::type>(false);
				if (std::is_const<T>::value)
				{
					if (std::is_volatile<T>::value)
						return valueTypeName + " * const volatile";
					else
						return valueTypeName + " * const";
				}
				else
				{
					if (std::is_volatile<T>::value)
						return valueTypeName + " * volatile";
					else
						return valueTypeName + " *";
				}
			}
			else if (std::is_lvalue_reference<T>::value)
			{
				String valueTypeName = _getTypeName<std::remove_reference<T>::type>(false);
				return valueTypeName + " &";
			}
			else if (std::is_rvalue_reference<T>::value)
			{
				String valueTypeName = _getTypeName<std::remove_reference<T>::type>(false);
				return valueTypeName + " &&";
			}
			else
			{
				static String valueTypeName = String(typeid(T).name())
					.replaceAll(" const ", " ")
					.replaceAll(" volatile ", " ")
					.replaceAll(" * __ptr64", "");

				if (std::is_const<T>::value)
				{
					if (std::is_volatile<T>::value)
						return ignoreModifiers ? valueTypeName : (valueTypeName + " const volatile");
					else
						return ignoreModifiers ? valueTypeName : (valueTypeName + " const");
				}
				else
				{
					if (std::is_volatile<T>::value)
						return ignoreModifiers ? valueTypeName : (valueTypeName + " volatile");
					else
						return valueTypeName;
				}
			}
		}
	};

} // namespace sonic
