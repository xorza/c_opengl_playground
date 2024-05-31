#include "graph.hpp"

#include "utils/utils.hpp"


#include <cassert>


Node::Node(Func &func) {
    id = generate_uuid();
    func_id = func.id;
    name = func.name;
    for (const auto &arg: func.args) {
        auto &input = inputs.emplace_back();
        input.binding = BindingType::None;
    }
    for (const auto &event: func.events) {
        auto &node_event = events.emplace_back();
    }

}

std::string to_string(const BindingType &binding_type) {
    switch (binding_type) {
        case BindingType::None:
            return "None";
        case BindingType::Const:
            return "Const";
        case BindingType::Binding:
            return "Binding";
    }
    assert(false);
}


YAML::Emitter &operator<<(YAML::Emitter &out, const Node &node) {
    out << YAML::BeginMap;
    out << YAML::Key << "id" << YAML::Value << to_string(node.id);
    out << YAML::Key << "func_id" << YAML::Value << to_string(node.func_id);
    out << YAML::Key << "name" << YAML::Value << node.name;
    out << YAML::Key << "is_output" << YAML::Value << node.is_output;
    out << YAML::Key << "cache_outputs" << YAML::Value << node.cache_outputs;

    out << YAML::Key << "inputs" << YAML::Value << YAML::BeginSeq;
    for (const auto &input: node.inputs) {
        out << YAML::BeginMap;
        out << YAML::Key << "binding" << YAML::Value << to_string(input.binding);
        switch (input.binding) {
            case BindingType::None:
                break;

            case BindingType::Const:
                assert(input.value.has_value());
                assert(false);
                break;

            case BindingType::Binding:
                out << YAML::Key << "output_node_id" << YAML::Value << to_string(input.output_node_id);
                out << YAML::Key << "output_idx" << YAML::Value << input.output_idx;
                break;
        }

        out << YAML::EndMap;
    }
    out << YAML::EndSeq;

    out << YAML::Key << "events" << YAML::Value << YAML::BeginSeq;
    for (const auto &event: node.events) {
        out << YAML::BeginMap;
        out << YAML::Key << "subscribers" << YAML::Value << YAML::BeginSeq;
        for (const auto &subscriber: event.subscribers) {
            out << to_string(subscriber);
        }
        out << YAML::EndSeq;
        out << YAML::EndMap;
    }
    out << YAML::EndSeq;

    out << YAML::EndMap;
    return out;
}

YAML::Emitter &operator<<(YAML::Emitter &out, const Graph &func) {
    out << YAML::BeginSeq;
    for (const auto &node: func.nodes) {
        out << node;
    }
    out << YAML::EndSeq;
    return out;
}
