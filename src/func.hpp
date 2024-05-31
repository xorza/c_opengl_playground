#pragma once


#include "utils/nocopy.hpp"

#include <uuid.h>
#include <yaml-cpp/yaml.h>

#include <string>
#include <vector>
#include <cstdint>


enum class ArgType : uint8_t {
    In, Out
};

struct Arg {
    std::string name;
    uint32_t datatype = 1;
    bool required = true;
    ArgType type = ArgType::In;
};

struct Event {
    std::string name;
};

enum class FuncBehavior : uint8_t {
    Pure,  // always return the same output for the same input
    Impure // may return different outputs for the same input
};

struct Func {
    NOCOPY(Func)

    uuids::uuid id;
    std::string name;
    FuncBehavior behavior = FuncBehavior::Impure;
    std::vector<Arg> args;
    std::vector<Event> events;

    Func();


};

YAML::Emitter &operator<<(YAML::Emitter &out, const Func &func);


