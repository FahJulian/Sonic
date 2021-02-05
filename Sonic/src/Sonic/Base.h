#pragma once
#include <memory>
#include <string>

namespace Sonic {

    template<typename T>
    using Ref = std::shared_ptr<T>;

    template<typename T, typename... Args>
    Ref<T> createRef(Args&&... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    extern const std::string& resourceDir();

    static const std::string& coreResourceDir()
    {
        static std::string coreResourceDir = "C:\\dev\\Sonic\\Sonic\\res\\";
        return coreResourceDir;
    }

}
