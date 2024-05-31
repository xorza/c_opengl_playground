#include "utils.hpp"


uuids::uuid_random_generator create_uuid_random_generator() {
    std::random_device rd;
    auto seed_data = std::array<int, std::mt19937::state_size>{};
    std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
    std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
    std::mt19937 generator(seq);
    uuids::uuid_random_generator gen{generator};
    return gen;
}

uuids::uuid generate_uuid() {
    static uuids::uuid_random_generator gen = create_uuid_random_generator();
    return gen();
}