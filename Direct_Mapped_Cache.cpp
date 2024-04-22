#include "Direct_Mapped_Cache.h"

DirectMappedCache::DirectMappedCache(Memory& memory, string type_of_cache, unsigned int delay, bool cache_enabled)
    : Cache(memory, type_of_cache, delay) {
    cache.fill({0, 0, 0, 0});
    cache_tag.fill(0);
    cache_valid.fill(false);
    cache_dirty.fill(false);
}

DirectMappedCache::~DirectMappedCache() {
    // Destructor
}

unsigned int DirectMappedCache::compute_line_index_from_address(unsigned int address) {
    // take out the index bits
    return (address & mask_for_index_bits) >> num_of_bits_for_offset;

}

unsigned int DirectMappedCache::compute_word_offset_from_address(unsigned int address) {
    return (address & mask_for_offset) >> num_of_bits_for_byte_offset;
}

unsigned int DirectMappedCache::compute_tag_from_address(unsigned int address) {
    // unsigned int temp = address >> (num_of_bits_for_offset + num_of_bits_for_index);
    // cout << format("address: {0:#0x}, temp: {1:#0x}\n", address, temp);
    return address >> (num_of_bits_for_offset + num_of_bits_for_index);
}

unsigned int DirectMappedCache::compute_address_from_tag_and_index(unsigned int tag, unsigned int line_index){
    return (tag << (num_of_bits_for_index + num_of_bits_for_offset)) | (line_index << num_of_bits_for_offset);
}

void DirectMappedCache::reset() {
    Cache::reset();
    cache.fill({0, 0, 0, 0});
    cache_tag.fill(0);
    cache_valid.fill(false);
    cache_dirty.fill(false);
}

bool DirectMappedCache::write_a_word(unsigned int address, unsigned int a_word) {
    //unsigned int addressMod = address % num_of_lines;
    if (cache_enabled) return write_a_word_if_cache_enabled(address, a_word);
    // if cache disabled, directly write to memory
    return memory.write_a_word_because_cache_disabled(type_of_cache, address, a_word);
}

bool DirectMappedCache::write_a_word_if_cache_enabled(unsigned int address, unsigned int a_word){
    address = convert_address_to_valid_memory_address(address);
    if(address_request_in_service == 0xFFFFFFFF){
        address_request_in_service = address;
        // need to decrement counter
    }else if(address_request_in_service != address){

        return false;

    }else if(cache_checked){ // address_request_in_service == address, same request again
        // but cache has been checked last time, obviously it was a miss
        unsigned int line_index = compute_line_index_from_address(address);
        unsigned int tag_of_src = compute_tag_from_address(address);
        unsigned int word_offset = compute_word_offset_from_address(address);

        // check whether need to write back to memory
        if((cache_valid)[line_index] && (cache_dirty)[line_index]){
            //compute the address of the evicted line
            unsigned int tag_of_evicted_line =  cache_tag[line_index];
            unsigned int address_of_evicted_line = compute_address_from_tag_and_index(tag_of_evicted_line, line_index);
            // write back to memory
            bool feedback = memory.write_a_line(type_of_cache, address_of_evicted_line, cache[line_index]);

            if(!feedback) {
                // memory is busy, need to wait
                return false;
            }

            // memory write back is done
            cache_dirty[line_index] = false;
            // (*cache_which_way_valid)[line_index] = false;
        }

        // either no need to write back to memory or memory write back is done
        auto&& [flag , result] = memory.read_a_line(type_of_cache, address);

        // flag = false, means the memory let me wait, memory is busy either serving my requet or another client's request
        if(!flag){
            return false;
        }
        // flag = true, means the memory is returning the data at this cycle
        cache[line_index] = result;
        cache[line_index][word_offset] = a_word;
        cache_tag[line_index] = tag_of_src;
        cache_valid[line_index] = true;
        cache_dirty[line_index] = true;
        //LRU_indicator = 1 - way_indicator;
        address_request_in_service = 0xFFFFFFFF;
        counter = delay;
        cache_checked = false;
        return true;
    }

    // either a new request, or same request again but cache unchecked
    // cache_checked = false
    counter --;
    if(counter > 0) return false;
    // counter = 0, move on to check cache

    unsigned int line_index = compute_line_index_from_address(address);
    unsigned int tag_of_src = compute_tag_from_address(address);
    unsigned int word_offset = compute_word_offset_from_address(address);


    if(cache_valid[line_index] && cache_tag[line_index] == tag_of_src) {
        // cout << __FILE__ << __LINE__ << endl;
        hit_count++;
        //LRU_indicator = 1;
        cache[line_index][word_offset] = a_word;
        cache_dirty[line_index] = true;
        address_request_in_service = 0xFFFFFFFF;
        counter = delay;
        cache_checked = false;
        return true;
    }

    // cout << __FILE__ << __LINE__ << endl;
    // miss
    miss_count++;
    cache_checked = true;
    return false;
}

tuple<bool, unsigned int> DirectMappedCache::read_a_word(unsigned int address) {
    if(cache_enabled) return read_a_word_if_cache_enabled(address);
    // if cache disabled, directly read from memory
    auto&& [flag , result] = memory.read_a_line(type_of_cache, address);
    // flag = false, means the memory let me wait, memory is busy either serving my requet or another client's request
    if(!flag){
        return {false, 0};
    }
    // flag = true, means the memory is returning the data at this cycle
    unsigned int word_offset = compute_word_offset_from_address(address);
    return {true, result[word_offset]};
}

tuple<bool, unsigned int> DirectMappedCache::read_a_word_if_cache_enabled(unsigned int address){
    address = convert_address_to_valid_memory_address(address);
    if(address_request_in_service == 0xFFFFFFFF){
        // can serve a new request from unit
        address_request_in_service = address;
        // need to decrement counter
    }else if(address_request_in_service != address){

        return {false, 0};

    }else if(cache_checked) { // address_request_in_service == address, same request again
        // but cache has been checked last time, obviously it was a miss
        unsigned int line_index = compute_line_index_from_address(address);
        unsigned int tag_of_src = compute_tag_from_address(address);
        unsigned int word_offset = compute_word_offset_from_address(address);

        // check whether need to write back to memory
        if(cache_valid[line_index] && cache_dirty[line_index]){
            //compute the address of the evicted line
            unsigned int tag_of_evicted_line =  cache_tag[line_index];
            unsigned int address_of_evicted_line = compute_address_from_tag_and_index(tag_of_evicted_line, line_index);
            // write back to memory
            bool feedback = memory.write_a_line(type_of_cache, address_of_evicted_line, cache[line_index]);

            if(!feedback) {
                // memory is busy, need to wait
                return {false, 0};
            }

            // memory write back is done
            cache_dirty[line_index] = false;
            // (*cache_which_way_valid)[line_index] = false;

            // return {false, 0};
        }

        // either no need to write back to memory or memory write back is done
        auto&& [flag , result] = memory.read_a_line(type_of_cache, address);

        // flag = false, means the memory let me wait, memory is busy either serving my requet or another client's request
        if(!flag){
            return {false, 0};
        }
        // flag = true, means the memory is returning the data at this cycle
        cache[line_index] = result;
        cache_tag[line_index] = tag_of_src;
        cache_valid[line_index] = true;
        cache_dirty[line_index] = false;
        //LRU_indicator = 1 - way_indicator;
        address_request_in_service = 0xFFFFFFFF;
        counter = delay;
        cache_checked = false;
        return {true, result[word_offset]};
    }

    // either a new request, or same request again but cache unchecked
    // cache_checked = false
    counter --;
    if(counter > 0) return {false, 0};
    // counter = 0, move on to check cache

    unsigned int line_index = compute_line_index_from_address(address);
    unsigned int tag_of_src = compute_tag_from_address(address);
    unsigned int word_offset = compute_word_offset_from_address(address);

    // hit
    if(cache_valid[line_index] && cache_tag[line_index] == tag_of_src) {
        hit_count++;
        //LRU_indicator = 1;
        address_request_in_service = 0xFFFFFFFF;
        counter = delay;
        cache_checked = false;
        return {true, cache[line_index][word_offset]};
    }
    // cout << __FILE__ << __LINE__ << endl;
    // miss
    miss_count++;
    cache_checked = true;
    return {false, 0};

}

vector<string> DirectMappedCache::print_segment_of_cache(unsigned int start_address, unsigned int num_of_lines) {
    start_address = convert_address_to_valid_memory_address(start_address);
    unsigned int start_line_index = compute_line_index_from_address(start_address);
    vector<string> output;
    unsigned int i = start_line_index;
    while(num_of_lines--){
        output.push_back(cache_valid[i] ? "true" : "false");
        output.push_back(cache_dirty[i] ? "true" : "false");
        output.push_back(std::to_string(cache_tag[i]));
        output.push_back(std::to_string(cache[i][0]));
        output.push_back(std::to_string(cache[i][1]));
        output.push_back(std::to_string(cache[i][2]));
        output.push_back(std::to_string(cache[i][3]));
        i++;
    }

    return output;
}
