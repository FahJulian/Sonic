#include <json/json.hpp>
#include "Sonic/App.h"
#include "Sonic/Util/StringUtils.h"
#define SONIC_CLIENT_SERIALIZATION_IMPORT
//#define SONIC_CLIENT_SERIALIZATION_RELEASE
#include "ClientSerialization.h"
#include "SceneSerializer.h"

using namespace Sonic;


template<typename Signature>
static Result<Ref<Callable<Signature>>, CallableDeserializationError>
deserializeMethod(Script* script, const SerializedCallable& method)
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
	Script* script = createClientScript("TestScript2");
	auto c = deserializeMethod<void(int)>(script, { "TestScript2", "TestFunc" });
	auto c2 = deserializeMethod<void(const UIEntityClickedEvent&)>(script, { "TestScript2", "OnClicked" });
	if (!c.HasError())
		(*c.GetResult())(3);

	script->Init(scene, 6);

	BaseCallable callable = (BaseCallable)*c.GetResult();

	Callable<void()>* func = new Function<void()>(App::stop);

	auto s = serializeCallable(*c.GetResult());
	auto s2 = serializeCallable(*c2.GetResult());
	auto s3 = serializeCallable(*func);

	std::vector<String> scriptFiles;
	std::vector<String> scriptClasses;
	std::vector<String> scriptConstructorCode;
	std::unordered_map<String, std::vector<String>> scriptMethods;

	scriptMethods.emplace("TestScript2", std::vector<String>({ "TestFunc", "OnClicked" }));

	String sceneName = relativeFilePath.substr(relativeFilePath.find_last_of('/') + 1);
	InputFileStream file = InputFileStream(resourceDir() + relativeFilePath + "/" + sceneName + ".sonicscene.json");

	JSON json;
	file >> json;

	for (auto& scriptFile : json["scripts"])
	{
		scriptFiles.push_back(scriptFile.get<String>());
		scriptClasses.push_back(Util::getFileNamePrefix(scriptFiles.back()));
	}

	String code;
	
#ifdef SONIC_CLIENT_SERIALIZATION_RELEASE
	code.append("#define SONIC_CLIENT_SERIALIZATION_RELEASE");
#endif

	code.append("#include \"Sonic/Scene/Serialization/ClientSerialization.h\"\n");

	for (auto& scriptFile : scriptFiles)
		code.append("#include \"" + resourceDir() + relativeFilePath + "/" + scriptFile + "\"\n");

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

	OutputFileStream codeFile = OutputFileStream("C:/dev/Sonic/ClientSceneSerialization/src/GeneratedClientSerialization.cpp");
	codeFile.write(code.c_str(), code.size());
	codeFile.close();

	// Compile

	std::vector<Script*> scripts;
	for (auto& scriptClass : scriptClasses)
	{
		SONIC_LOG_DEBUG("1");

		Script* script = createClientScript(scriptClass);
		script->OnInit();
		scripts.push_back(script);

		SONIC_LOG_DEBUG("2");
	}
}

void SceneSerializer::serialize(Scene* scene, const String& relativeFilePath)
{

}
