#pragma once
#include <memory>
#include <string>

#define SONIC_RESOURCE_DIR std::string("C:\\dev\\Sonic\\Sonic\\res\\")

namespace Sonic {

    template<typename T>
    using Ref = std::shared_ptr<T>;

    extern const std::string& resourceDir();

}
