#include "src/func.hpp"
#include "src/graph.hpp"

#include <iostream>
#include <string>
#include <yaml-cpp/yaml.h>

#include <catch2/catch_test_macros.hpp>


TEST_CASE("Sample test case", "[addition]") {
    REQUIRE(1 + 1 == 2);
}


struct Person {
    std::string name;
    int age;
    std::string city;
};

YAML::Emitter &operator<<(YAML::Emitter &out, const Person &person) {
    out << YAML::BeginMap;
    out << YAML::Key << "name" << YAML::Value << person.name;
    out << YAML::Key << "age" << YAML::Value << person.age;
    out << YAML::Key << "city" << YAML::Value << person.city;
    out << YAML::EndMap;
    return out;
}

TEST_CASE("Yaml serialization", "[serialization]") {
    Person person = {"John Doe", 30, "New York"};

    YAML::Emitter out;
    out << person;

    std::cout << "YAML Output:\n" << out.c_str() << std::endl;
}

Func make_func() {
    Func func = Func{};
    func.name = "func1";
    func.args.push_back(FuncArg{"arg1", 1, true, FuncArgType::In});
    func.args.push_back(FuncArg{"arg2", 2, false, FuncArgType::Out});
    func.events.push_back(FuncEvent{"event1"});
    func.events.push_back(FuncEvent{"event2"});
    return func;
}

TEST_CASE("Func serialization", "[serialization]") {
    Func func = make_func();

    YAML::Emitter out;
    out << func;

    std::cout << "YAML Output:\n" << out.c_str() << std::endl;
}

TEST_CASE("Node deserialization", "[serialization]") {
    Func func = make_func();
    Node node = Node(func);

    YAML::Emitter out;
    out << node;

    std::cout << "YAML Output:\n" << out.c_str() << std::endl;
}