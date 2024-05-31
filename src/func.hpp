#pragma once


#include "utils/nocopy.hpp"

#include <uuid.h>
#include <yaml-cpp/yaml.h>

#include <string>
#include <vector>
#include <cstdint>


using FuncId = uuids::uuid;

enum class FuncArgType : uint8_t {
    In, Out
};

struct FuncArg {
    std::string name;
    uint32_t datatype = 1;
    bool required = true;
    FuncArgType type = FuncArgType::In;
};

struct FuncEvent {
    std::string name;
};

enum class FuncBehavior : uint8_t {
    Pure,  // always return the same output for the same input
    Impure // may return different outputs for the same input
};

struct Func {
    FuncId id;
    std::string name;
    FuncBehavior behavior = FuncBehavior::Impure;
    std::vector<FuncArg> args;
    std::vector<FuncEvent> events;

    Func();


};

YAML::Emitter &operator<<(YAML::Emitter &out, const Func &func);


