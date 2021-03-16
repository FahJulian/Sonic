#pragma once
#include <sstream>
#include "Sonic/Base.h"
#include "Sonic/Util/StringUtils.h"
#include "ClientSerializationGenerator.h"

using namespace Sonic;
using namespace Sonic::Serialization;


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

static void recompileDynamically(const String& codeFilePath)
{
}

static void recompileStatically(const String& codeFilePath)
{
}

void ClientSerializationGenerator::generateClientSerializationCode(const std::vector<String>& scripts, const String& relativeSceneFilePath, bool releaseBuild)
{
	std::vector<String> scriptClasses;
	std::unordered_map<String, std::vector<String>> scriptMethods;

	for (auto& filePath : scripts)
	{
		String className = Util::getFileNamePrefix(filePath);

		scriptClasses.push_back(className);
		scriptMethods.emplace(className, findMethodsInScript(filePath, className));
	}
	
	String code;

	if (releaseBuild)
		code.append("#define SONIC_CLIENT_SERIALIZATION_RELEASE");

	code.append("#include \"Sonic/Scene/Serialization/ClientSerialization.h\"\n");

	for (auto& scriptFile : scripts)
		code.append("#include \"" + scriptFile + "\"\n");

	code.append("\nusing namespace Sonic::Serialization;\n\n\n");

	code.append("Optional<Script*> Sonic::createClientScript(const String& scriptClass, const std::unordered_map<String, String>& data)\n");
	code.append("{\n");

	for (auto& scriptClass : scriptClasses)
	{
		code.append("    if (scriptClass == \"" + scriptClass + "\")\n");
		code.append("        return new " + scriptClass + "();\n");
	}

	code.append("\n    ");
	code.append("return { };\n");

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

	String codeFilePath = "C:/dev/Sonic/ClientSceneSerialization/src/ClientSerialization.cpp";
	OutputFileStream codeFile = OutputFileStream(codeFilePath);
	codeFile.write(code.c_str(), code.size());
	codeFile.close();

	if (releaseBuild)
		recompileStatically(codeFilePath);
	else
		recompileDynamically(codeFilePath);
}
