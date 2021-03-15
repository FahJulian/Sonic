#include "Sonic/Scene/Serialization/ClientSerialization.h"
#include "C:/dev/Sonic/Sandbox/res/scenes/sandbox/scripts/TestScript.hpp"
#include "C:/dev/Sonic/Sandbox/res/scenes/sandbox/scripts/TestScript2.hpp"

using namespace Sonic;


Script* Sonic::createClientScript(const String& scriptClass)
{
    if (scriptClass == "TestScript")
        return new TestScript();
    if (scriptClass == "TestScript2")
        return new TestScript2();

    return nullptr;
}

Optional<SerializedCallable> Sonic::serializeClientMethod(const BaseCallable& method)
{
    if (method.IsMethodOrFunction(&TestScript::OnInit))
        return { "TestScript", "OnInit" };
    if (method.IsMethodOrFunction(&TestScript::OnClick))
        return { "TestScript", "OnClick" };
    if (method.IsMethodOrFunction(&TestScript::OnUpdate))
        return { "TestScript", "OnUpdate" };
    if (method.IsMethodOrFunction(&TestScript2::TestFunc))
        return { "TestScript2", "TestFunc" };
    if (method.IsMethodOrFunction(&TestScript2::OnClicked))
        return { "TestScript2", "OnClicked" };
    if (method.IsMethodOrFunction(&TestScript2::OnInit))
        return { "TestScript2", "OnInit" };

    return { }; 
}

Result<uintptr_t, CallableDeserializationError>
    Sonic::deserializeClientMethod(Script* script, const SerializedCallable& method, const CallableSignature& signature)
{
    if (method.scriptClass == "TestScript")
    {
        if (method.callable == "OnInit")
            return assureSignatureAndCreateMethod(script, &TestScript::OnInit, signature);
        if (method.callable == "OnClick")
            return assureSignatureAndCreateMethod(script, &TestScript::OnClick, signature);
        if (method.callable == "OnUpdate")
            return assureSignatureAndCreateMethod(script, &TestScript::OnUpdate, signature);
    }
    if (method.scriptClass == "TestScript2")
    {
        if (method.callable == "TestFunc")
            return assureSignatureAndCreateMethod(script, &TestScript2::TestFunc, signature);
        if (method.callable == "OnClicked")
            return assureSignatureAndCreateMethod(script, &TestScript2::OnClicked, signature);
        if (method.callable == "OnInit")
            return assureSignatureAndCreateMethod(script, &TestScript2::OnInit, signature);
    }

    return CallableDeserializationError::NotFound;
}
