#include "memory.h"



Memory::Memory(unsigned int& delay)
    : address_request_in_service(0xFFFFFFFF), curr_client(""),
    delay(delay), counter(delay){
    memory.fill({0, 0, 0, 0});
    //cache_counters.clear();
}

void Memory::reset(){
    memory.fill({0, 0, 0, 0});
    counter = delay;
    curr_client.clear();
    address_request_in_service = 0xFFFFFFFF;
}

unsigned int Memory::convert_address_to_valid_memory_address(unsigned int address){
    return  (address & mask_for_index_plus_offset);
}

bool Memory::cancel_in_service_request(string caller, unsigned int address){
    if(curr_client.empty()) return true; // no request in service
    address = convert_address_to_valid_memory_address(address);
    if(curr_client == caller && address_request_in_service == address){
        address_request_in_service = 0xFFFFFFFF;
        curr_client.clear();
        counter = delay;
        return true;
    }

    return false;
}

unsigned int Memory::compute_line_index_from_address(unsigned int address) {
    // take out the index bits
    return (address & mask_for_index_bits) >> num_of_bits_for_offset;
}

unsigned int Memory::compute_word_offset_from_address(unsigned int address) {
    return (address & mask_for_offset) >> num_of_bits_for_byte_offset;
}

bool Memory::write_a_word_because_cache_disabled(string caller, unsigned int address, unsigned int a_word){
    address = convert_address_to_valid_memory_address(address);
    if(curr_client.empty() || (curr_client == caller && address_request_in_service == address)) {
        curr_client = caller;
        address_request_in_service = address;
        counter--;
        if(counter > 0) return false;

        unsigned int line_index = compute_line_index_from_address(address);
        unsigned int word_offset = compute_word_offset_from_address(address);
        memory[line_index][word_offset] = a_word;
        address_request_in_service = 0xFFFFFFFF;
        curr_client.clear();
        counter = delay;
        return true;

    }
    // either curr_client (not empty) != caller
    // or curr_client == caller but address_request_in_service != address
    return false;
}

/* memory always write a line of data, never a single word */
bool Memory::write_a_line(string caller, unsigned int address, array<unsigned int, memory_constants::num_of_words>& cach_line) {
    address = convert_address_to_valid_memory_address(address);
    if(curr_client.empty() || (curr_client == caller && address_request_in_service == address)) {
        curr_client = caller;
        address_request_in_service = address;
        counter--;
        if(counter > 0) return false;

        unsigned int line_index = compute_line_index_from_address(address);
        memory[line_index] = cach_line;
        address_request_in_service = 0xFFFFFFFF;
        curr_client.clear();
        counter = delay;
        return true;

    }
    // either curr_client (not empty) != caller
    // or curr_client == caller but address_request_in_service != address

    return false;
}

/* memory always read a line of data, never a single word */
tuple<bool, array<unsigned int, memory_constants::num_of_words>> Memory::read_a_line(string caller, unsigned int address) {
    address = convert_address_to_valid_memory_address(address);
    if(curr_client.empty() || (curr_client == caller && address_request_in_service == address)) {
        curr_client = caller;
        address_request_in_service = address;
        counter--;
        if(counter > 0) return {false, { 0, 0, 0, 0 }};

        unsigned int line_index = compute_line_index_from_address(address);
        address_request_in_service = 0xFFFFFFFF;
        curr_client.clear();
        counter = delay;
        return {true, memory[line_index]};
    }
    // either curr_client (not empty) != caller
    // or curr_client == caller but address_request_in_service != address
    return {false, { 0, 0, 0, 0 }};

}

// length_in_words indicates the number of words to read from the start_address.
vector<unsigned int> Memory::get_memory_segment(unsigned int start_address, unsigned int length_in_words) {
    start_address = convert_address_to_valid_memory_address(start_address);
    unsigned int start_line_index = compute_line_index_from_address(start_address);
    unsigned int num_of_lines = ceil(static_cast<double>(length_in_words) / memory_constants::num_of_words);
    unsigned int i = start_line_index;
    vector<unsigned int> segment;
    while(num_of_lines--){
        auto  memory_line = memory[i];
        segment.push_back(memory_line[0]);
        segment.push_back(memory_line[1]);
        segment.push_back(memory_line[2]);
        segment.push_back(memory_line[3]);
        i++;
    }

    return segment;
}

void Memory::load_data_segment_into_memory(unsigned int start_address, vector<array<unsigned int, memory_constants::num_of_words>>& segment) {
    start_address = convert_address_to_valid_memory_address(start_address);
    unsigned int line_index = compute_line_index_from_address(start_address);

    for(auto& line : segment) {
        memory[line_index++] = line;
    }
}

array<string, 5*(memory_constants::num_of_lines)> Memory::print_segment_of_memory(unsigned int start_address, unsigned int num_of_lines) {
    start_address = convert_address_to_valid_memory_address(start_address);
    unsigned int start_line_index = compute_line_index_from_address(start_address);
    array<string, 5*(memory_constants::num_of_lines)> output;

    unsigned int i = start_line_index;
    int k = 0;
    while (num_of_lines--){
        //output += memory[i][0]+memory[i][1]+memory[i][2]+memory[i][3];
        output[k*5] = std::to_string(start_address);
        output[k*5+1] = std::to_string(memory[i][0]);
        output[k*5+2] = std::to_string(memory[i][1]);
        output[k*5+3] = std::to_string(memory[i][2]);
        output[k*5+4] = std::to_string(memory[i][3]);
        //output += format("{0:#0{5}x} | {1:#0{5}d} | {2:#0{5}d} | {3:#0{5}d} | {4:#0{5}d}\n",
//                         (i << num_of_bits_for_offset) , memory[i][0], memory[i][1], memory[i][2], memory[i][3], 10);
        i++;
        k++;
    }
    return output;
}

void Memory::write_a_line_as_side_door(unsigned int address, array<unsigned int, memory_constants::num_of_words>& a_line_of_data) {
    address = convert_address_to_valid_memory_address(address);
    unsigned int line_index = compute_line_index_from_address(address);
    memory[line_index] = a_line_of_data;

    return;

}
