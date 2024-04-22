#pragma once
#ifndef MEMORY_H
#define MEMORY_H

#include <array>
#include <string>
#include <map>
#include <tuple>
#include <vector>
#include <cmath>
#include <iostream>
#include <format>

using std::string, std::array, std::map, std::tuple, std::vector,
    std::cout, std::endl, std::format, std::ceil;

constexpr unsigned int bytes_per_word = 4;

namespace memory_constants {
static constexpr unsigned int num_of_words = 4;
static constexpr unsigned int num_of_lines = 16384;
}


class Memory {
public:
    Memory(unsigned int& delay);

    // Other class members
    unsigned int convert_address_to_valid_memory_address(unsigned int address);

    bool write_a_line(string caller, unsigned int address, array<unsigned int, memory_constants::num_of_words>& cach_line);

    tuple<bool, array<unsigned int, memory_constants::num_of_words>> read_a_line(string caller, unsigned int address);

    vector<unsigned int>  get_memory_segment(unsigned int start_address, unsigned int len_in_word);

    void load_data_segment_into_memory(unsigned int start_address, vector<array<unsigned int, memory_constants::num_of_words>>& segment);

    unsigned int compute_line_index_from_address(unsigned int address);

    unsigned int compute_word_offset_from_address(unsigned int address);

    bool write_a_word_because_cache_disabled(string caller, unsigned int address, unsigned int a_word);

    array<string, 5*(memory_constants::num_of_lines)> print_segment_of_memory(unsigned int start_address, unsigned int num_of_lines);

    bool cancel_in_service_request(string caller, unsigned int address);

    void reset();

    void write_a_line_as_side_door(unsigned int address, array<unsigned int, memory_constants::num_of_words>& a_line_of_data);

private:

    array<array<unsigned int, memory_constants::num_of_words>, memory_constants::num_of_lines> memory;
    // 16
    static constexpr unsigned int len_of_a_line_in_bytes = memory_constants::num_of_words * bytes_per_word;
    // 4
    static constexpr unsigned int num_of_bits_for_offset = __builtin_ctz(len_of_a_line_in_bytes);
    // 2
    static constexpr unsigned int num_of_bits_for_byte_offset = __builtin_ctz(bytes_per_word);
    // 14
    static constexpr unsigned int num_of_bits_for_index = __builtin_ctz(memory_constants::num_of_lines);

    static constexpr unsigned int zero = 0;
    // 14
    static constexpr unsigned int num_of_upper_bits = 32 - num_of_bits_for_offset - num_of_bits_for_index;
    // 0x3FFF0
    static constexpr unsigned int mask_for_index_bits = (((~zero) << (num_of_upper_bits))
                                                         >> (num_of_upper_bits + num_of_bits_for_offset))
                                                        << (num_of_bits_for_offset);
    // 18
    static constexpr unsigned int mask_for_index_plus_offset = (1 << (num_of_bits_for_offset + num_of_bits_for_index)) -1;
    //
    static constexpr unsigned int mask_for_offset = (1 << num_of_bits_for_offset) - 1;

    unsigned int counter;
    // map<string, unsigned int> cache_counters;
    unsigned int address_request_in_service;
    string curr_client;
    unsigned int delay;
};

#endif // MEMORY_H
