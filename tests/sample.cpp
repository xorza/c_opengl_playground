#include "src/func.hpp"

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


TEST_CASE("Func serialization", "[serialization]") {
    Func func = Func{};
    func.name = "func1";
    func.args.push_back(Arg{"arg1", 1, true, ArgType::In});
    func.args.push_back(Arg{"arg2", 2, false, ArgType::Out});
    func.events.push_back(Event{"event1"});
    func.events.push_back(Event{"event2"});

    YAML::Emitter out;
    out << func;

    std::cout << "YAML Output:\n" << out.c_str() << std::endl;
}