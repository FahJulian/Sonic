#pragma once
#include <memory>
#include <string>

namespace Sonic {

    using String = std::string;

    template<typename T>
    using Ref = std::shared_ptr<T>;

    template<typename T, typename... Args>
    Ref<T> createRef(Args&&... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    extern const String& resourceDir();

    static const String& coreResourceDir()
    {
        static String coreResourceDir = "C:\\dev\\Sonic\\Sonic\\res\\";
        return coreResourceDir;
    }

}
