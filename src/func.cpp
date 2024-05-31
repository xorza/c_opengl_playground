#include "func.hpp"

#include "utils/utils.hpp"


#include <yaml-cpp/yaml.h>

#include <cassert>


Func::Func() {
    id = generate_uuid();
}


std::string to_string(const FuncBehavior &func_behavior) {
    switch (func_behavior) {
        case FuncBehavior::Pure:
            return "Pure";
        case FuncBehavior::Impure:
            return "Impure";
    }
    assert(false);
}

std::string to_string(const FuncArgType &arg_type) {
    switch (arg_type) {
        case FuncArgType::In:
            return "In";
        case FuncArgType::Out:
            return "Out";
    }
    assert(false);
}

YAML::Emitter &operator<<(YAML::Emitter &out, const Func &func) {
    out << YAML::BeginMap;
    out << YAML::Key << "id" << YAML::Value << to_string(func.id);
    out << YAML::Key << "name" << YAML::Value << func.name;
    out << YAML::Key << "behavior" << YAML::Value << to_string(func.behavior);

    out << YAML::Key << "args" << YAML::Value << YAML::BeginSeq;
    for (const auto &arg: func.args) {
        out << YAML::BeginMap;
        out << YAML::Key << "name" << YAML::Value << arg.name;
        out << YAML::Key << "datatype" << YAML::Value << arg.datatype;
        out << YAML::Key << "required" << YAML::Value << arg.required;
        out << YAML::Key << "type" << YAML::Value << to_string(arg.type);
        out << YAML::EndMap;
    }
    out << YAML::EndSeq;

    out << YAML::Key << "events" << YAML::Value << YAML::BeginSeq;
    for (const auto &event: func.events) {
        out << YAML::BeginMap;
        out << YAML::Key << "name" << YAML::Value << event.name;
        out << YAML::EndMap;
    }
    out << YAML::EndSeq;

    out << YAML::EndMap;
    return out;
}
