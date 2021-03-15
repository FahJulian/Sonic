#include <json/json.hpp>
#include <sstream>
#include "Sonic/App.h"
#include "Sonic/Util/StringUtils.h"
#define SONIC_CLIENT_SERIALIZATION_IMPORT
//#define SONIC_CLIENT_SERIALIZATION_RELEASE
#include "ClientSerialization.h"
#include "SceneSerializer.h"

using namespace Sonic;


static std::vector<String> findMethodsInScript(const String& scriptFile, const String& scriptClass)
{
	std::vector<String> methods;

	InputFileStream file = InputFileStream(scriptFile);
	std::stringstream code;
	code << file.rdbuf();

	bool clear = false;
	bool inScriptClass = false;
	bool isPublic = false;
	bool inConstructor = false;
	std::vector<char> lastIdentifier;

	char c = ' ';
	int level = 0;
	while (code.read(&c, 1))
	{
		if (c == '{')
		{
			if (level == 0)
				inScriptClass = lastIdentifier.back() != ')';	// No function == in script class for now

			level++;
		}
		else if (c == '}')
		{
			if (level == 1)
				inScriptClass = false;
			else if (level == 2)
				inConstructor = false;

			level--;
		}

		if (inScriptClass && level == 1 && c == '(')
		{
			String methodName = { lastIdentifier.data(), lastIdentifier.size() };
			lastIdentifier.clear();

			if (methodName == scriptClass)	// Constructor
				inConstructor = true;
			else if (!inConstructor && isPublic)
				methods.push_back(methodName);
		}

		if (c == ' ' || c == '\n' || c == '\t' || c == '<' || c == '>')
		{
			if (level == 0 && String{ lastIdentifier.data(), lastIdentifier.size() } == "struct")
				isPublic = true;

			clear = true;
		}
		else if (level == 1 && c == ':')
		{
			String identifier = { lastIdentifier.data(), lastIdentifier.size() };

			if (identifier == "public")
				isPublic = true;
			else if (identifier == "private" || identifier == "protected")
				isPublic = false;
		}
		else
		{
			if (clear)
			{
				lastIdentifier.clear();
				clear = false;
			}

			lastIdentifier.push_back(c);
		}
	}

	return methods;
}

template<typename Signature>
static Result<Ref<Callable<Signature>>, CallableDeserializationError>
	deserializeCallable(Script* script, const SerializedCallable& method)
{
	if (Result<uintptr_t, CallableDeserializationError> result = deserializeClientMethod(script, method, getSignature<Signature>());
		result.HasError())
	{
		return result.GetError();
	}
	else
	{
		return Ref<Callable<Signature>>(reinterpret_cast<Callable<Signature>*>(result.GetResult()));
	}
}

Optional<SerializedCallable> serializeCallable(const BaseCallable& callable)
{
	if (callable.IsMethodOrFunction(App::stop))
		return "App::stop";

	return serializeClientMethod(callable);
}


void SceneSerializer::deserialize(Scene* scene, const String& relativeFilePath)
{
	std::vector<String> scriptFiles;
	std::vector<String> scriptClasses;
	std::unordered_map<String, std::vector<String>> scriptMethods;

	String sceneName = relativeFilePath.substr(relativeFilePath.find_last_of('/') + 1);
	InputFileStream file = InputFileStream(resourceDir() + relativeFilePath + "/" + sceneName + ".sonicscene.json");

	JSON json;
	file >> json;

	for (auto& scriptFile : json["scripts"])
	{
		String filePath = resourceDir() + relativeFilePath + "/" + scriptFile.get<String>();
		String className = Util::getFileNamePrefix(filePath);

		scriptFiles.push_back(filePath);
		scriptClasses.push_back(className);
		scriptMethods.emplace(className, findMethodsInScript(filePath, className));
	}

	{
		String code;

#ifdef SONIC_CLIENT_SERIALIZATION_RELEASE
		code.append("#define SONIC_CLIENT_SERIALIZATION_RELEASE");
#endif

		code.append("#include \"Sonic/Scene/Serialization/ClientSerialization.h\"\n");

		for (auto& scriptFile : scriptFiles)
			code.append("#include \"" + scriptFile + "\"\n");

		code.append("\nusing namespace Sonic;\n\n\n");

		code.append("Script* Sonic::createClientScript(const String& scriptClass)\n");
		code.append("{\n");

		for (auto& scriptClass : scriptClasses)
		{
			code.append("    if (scriptClass == \"" + scriptClass + "\")\n");
			code.append("        return new " + scriptClass + "();\n");
		}

		code.append("\n    ");
		code.append("return nullptr;\n");

		code.append("}\n\n");

		code.append("Optional<SerializedCallable> Sonic::serializeClientMethod(const BaseCallable& method)\n");
		code.append("{\n");

		for (auto& [scriptClass, methods] : scriptMethods)
		{
			for (auto& method : methods)
			{
				code.append("    if (method.IsMethodOrFunction(&" + scriptClass + "::" + method + "))\n");
				code.append("        return { \"" + scriptClass + "\", \"" + method + "\" };\n");
			}
		}

		code.append("\n");
		code.append("    return { }; \n");
		code.append("}\n\n");

		code.append("Result<uintptr_t, CallableDeserializationError>\n");
		code.append("    Sonic::deserializeClientMethod(Script* script, const SerializedCallable& method, const CallableSignature& signature)\n");
		code.append("{\n");

		for (auto& [scriptClass, methods] : scriptMethods)
		{
			code.append("    if (method.scriptClass == \"" + scriptClass + "\")\n");
			code.append("    {\n");

			for (auto& method : methods)
			{
				code.append("        if (method.callable == \"" + method + "\")\n");
				code.append("            return assureSignatureAndCreateMethod(script, &" + scriptClass + "::" + method + ", signature);\n");
			}

			code.append("    }\n");
		}

		code.append("\n    return CallableDeserializationError::NotFound;\n");
		code.append("}\n");

		OutputFileStream codeFile = OutputFileStream("C:/dev/Sonic/ClientSceneSerialization/src/ClientSerialization.cpp");
		codeFile.write(code.c_str(), code.size());
		codeFile.close();

	}

	// Compile
}

void SceneSerializer::serialize(Scene* scene, const String& relativeFilePath)
{

}
