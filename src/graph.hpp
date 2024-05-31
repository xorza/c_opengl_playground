#pragma once

#include "func.hpp"

#include "utils/nocopy.hpp"

#include <uuid.h>
#include <yaml-cpp/yaml.h>

#include <vector>
#include <cstdint>

using NodeId = uuids::uuid;

enum class BindingType : uint8_t {
    None, Const, Binding,
};

struct NodeInput {

    BindingType binding = BindingType::None;
    std::optional<void *> value = std::nullopt;
    uuids::uuid output_node_id{};
    uint32_t output_idx = 0;

    NodeInput() = default;


};

struct NodeEvent {
    std::vector<NodeId> subscribers;

    NodeEvent() = default;
};

struct Node {
    NodeId id{};
    FuncId func_id{};
    std::string name;

    bool is_output = false;
    bool cache_outputs = false;

    std::vector<NodeInput> inputs;
    std::vector<NodeEvent> events;

    explicit Node(Func &func);
};

struct Graph {
    NOCOPY(Graph)

    std::vector<Node> nodes;
};


YAML::Emitter &operator<<(YAML::Emitter &out, const Graph &func);

YAML::Emitter &operator<<(YAML::Emitter &out, const Node &node);