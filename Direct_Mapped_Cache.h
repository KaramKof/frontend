#pragma once

#ifndef DIRECT_MAPPED_CACHE_H
#define DIRECT_MAPPED_CACHE_H

#include "Cache.h"

//static constexpr unsigned int num_of_words = 4;
//static constexpr unsigned int num_of_lines = 1024;

class DirectMappedCache : public Cache {
public:
    DirectMappedCache(Memory& memory, string type_of_cache, unsigned int delay = 1, bool cache_enabled = true);  // Default constructor
    ~DirectMappedCache(); // Default destructor

    void reset();

    unsigned int compute_line_index_from_address(unsigned int address);

    unsigned int compute_word_offset_from_address(unsigned int address);

    unsigned int compute_tag_from_address(unsigned int address);
    // for computing address of evicted cache line
    unsigned int compute_address_from_tag_and_index(unsigned int tag, unsigned int line_index);

    bool write_a_word(unsigned int address, unsigned int a_word);

    bool write_a_word_if_cache_enabled(unsigned int address, unsigned int a_word);

    tuple<bool, unsigned int> read_a_word(unsigned int address);

    tuple<bool, unsigned int> read_a_word_if_cache_enabled(unsigned int address);

    vector<string> print_segment_of_cache(unsigned int start_address, unsigned int num_of_lines);


private:
    static constexpr unsigned int num_of_words = 4;
    static constexpr unsigned int num_of_lines = 1024;
    array<array<unsigned int, num_of_words>, num_of_lines> cache;
    array<unsigned int, num_of_lines> cache_tag;
    array<bool, num_of_lines> cache_valid;
    array<bool, num_of_lines> cache_dirty;
    //2
    static constexpr unsigned int num_of_bits_for_byte_offset = __builtin_ctz(bytes_per_word);
    // 16
    static constexpr unsigned int len_of_a_line_in_bytes = num_of_words * bytes_per_word;
    // 4, offset = word offset | byte offset
    static constexpr unsigned int num_of_bits_for_offset = __builtin_ctz(len_of_a_line_in_bytes);
    // 9
    static constexpr unsigned int num_of_bits_for_index = __builtin_ctz(num_of_lines);

    static constexpr unsigned int zero = 0;
    // 19
    static constexpr unsigned int num_of_upper_bits = 32 - num_of_bits_for_offset - num_of_bits_for_index;
    // 0x3FFF0
    static constexpr unsigned int mask_for_index_bits = (((~zero) << (num_of_upper_bits))
                                                         >> (num_of_upper_bits + num_of_bits_for_offset))
                                                        << (num_of_bits_for_offset);

    static constexpr unsigned int mask_for_offset = (1 << num_of_bits_for_offset) - 1;

    unsigned int LRU_indicator;


};

#endif // DIRECT_MAPPED_CACHE_H
