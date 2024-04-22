#include "cache.h"

Cache::Cache(Memory& memory, string type_of_cache, unsigned int delay, bool cache_enabled)
    : memory(memory), type_of_cache(type_of_cache), hit_count(0), miss_count(0), counter(delay),
    address_request_in_service(0xFFFFFFFF), cache_checked(false), delay(delay), cache_enabled(cache_enabled){

}

Cache::~Cache() {
    // Destructor
}

void Cache::reset() {

    hit_count = 0;
    miss_count = 0;
    cache_checked = false;
    address_request_in_service = 0xFFFFFFFF;
    counter = delay;
}

unsigned int Cache::convert_address_to_valid_memory_address(unsigned int address){
    return memory.convert_address_to_valid_memory_address(address);

}

bool Cache::cancel_in_service_request(unsigned int address){
    if(address_request_in_service == 0xFFFFFFFF) return true; // no request in service
    address = convert_address_to_valid_memory_address(address);
    if (address_request_in_service == address) {
        memory.cancel_in_service_request(type_of_cache, address);
        address_request_in_service = 0xFFFFFFFF;
        counter = delay;
        cache_checked = false;
        return true;
    };
    // address_request_in_service != address
    return false;
}

void Cache::set_cache_enabled(){
    cache_enabled = true;
}

void Cache::set_cache_disabled(){
    cache_enabled = false;
}

unsigned int Cache::get_hit_count(){
    return hit_count;
}

unsigned int Cache::get_miss_count(){
    return miss_count;
}
