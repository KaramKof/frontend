#pragma once
#ifndef CACHE_H
#define CACHE_H

#include "memory.h"


class Cache {
public:
    Cache(Memory& memory, string type_of_cache, unsigned int delay = 1, bool cache_enabled = true);  // Default constructor
    virtual ~Cache() = 0; // pure virtual Default destructor

    virtual void reset();

    // address space is larger than memory, need to zero out some upper bits of address
    unsigned int convert_address_to_valid_memory_address(unsigned int address);

    bool cancel_in_service_request(unsigned int address);

    void set_cache_enabled();

    void set_cache_disabled();

    virtual bool write_a_word(unsigned int address, unsigned int a_word) = 0;

    virtual tuple<bool, unsigned int> read_a_word(unsigned int address) = 0;

    unsigned int get_hit_count();

    unsigned int get_miss_count();

    // virtual void print_segment_of_cache(unsigned int start_address, unsigned int num_of_lines) = 0;



protected:
    Memory& memory;
    string type_of_cache;
    unsigned int hit_count;
    unsigned int miss_count;
    unsigned int address_request_in_service;
    unsigned int counter;
    unsigned int delay;
    bool cache_checked;
    bool cache_enabled;
};

#endif // CACHE_H
