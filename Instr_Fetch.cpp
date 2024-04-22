#include "Instr_Fetch.h"


InstrFetch::InstrFetch(ProgramCounter& pc, Cache& instr_cache, bool pipeline_enabled)
    : program_counter(pc), instr_cache(instr_cache), pipeline_enabled(pipeline_enabled),
    is_curr_instr_empty(true), curr_instr(0), on_off_switch(false){
    // Initialize any other members of Instr_Fetch here
    if(!pipeline_enabled){ // pipeline disabled
        on_off_switch = true;
    }
}

void InstrFetch::reset(){
    is_curr_instr_empty = true;
    curr_instr = 0;
    on_off_switch = false;
    if(!pipeline_enabled){ // pipeline disabled
        on_off_switch = true;
    }
    return;
}


void InstrFetch::set_pipeline_disabled(){
    pipeline_enabled = false;
    on_off_switch = true;
    return;
}

void InstrFetch::set_pipeline_enabled(){
    pipeline_enabled = true;
    on_off_switch = false;
    return;
}

void InstrFetch::turn_off_switch(){
    on_off_switch = false;
    return;
}

void InstrFetch::turn_on_switch(){
    on_off_switch = true;
    return;
}

// tuple<is_valid_instr, instr_word, instr_addr>
tuple<bool, unsigned int, unsigned int> InstrFetch::proceed(string signal){
    if(!pipeline_enabled && !on_off_switch){
        return {false, 0, 0}; // return NO-OP
    }

    // either pipeline_enabled == true or pipeline_enabled == false && on_off_switch == true
    if(signal == "flush"){
        unsigned int prev_instr_addr = program_counter.get_previous_instruction_address();
        bool ans = instr_cache.cancel_in_service_request(prev_instr_addr);
        if(!ans){
            //cout << "Error from InstrFetch::proceed(string signal): cancel_in_service_request returned false" << endl;
        }

        is_curr_instr_empty = true;
        curr_instr = 0;
        return {false, 0, 0}; // return NO-OP
    }

    // signal not "flush"
    if(is_curr_instr_empty){
        unsigned int curr_instr_addr = program_counter.get_current_instruction_address();
        auto&&[response, word] = instr_cache.read_a_word(curr_instr_addr);
        if(response){
            if (signal == "permitted"){
                program_counter.increment_instruction_address_by_a_word();
                is_curr_instr_empty = true;
                curr_instr = 0;
                if(!pipeline_enabled) on_off_switch = false;
                return {true, word, curr_instr_addr};

            } else if (signal == "blocked"){
                is_curr_instr_empty = false;
                curr_instr = word;
                return {false, 0, 0};
            }
            // this line should never be reached
            throw "Error from InstrFetch::proceed(string signal): invalid signal: ";
        }
        // response == false
        return {false, 0, 0};

    }
    // is_curr_instr_empty == false
    if(signal == "permitted"){
        unsigned int curr_instr_addr = program_counter.get_current_instruction_address();
        program_counter.increment_instruction_address_by_a_word();
        is_curr_instr_empty = true;
        unsigned int temp = curr_instr;
        curr_instr = 0;
        return {true, temp, curr_instr_addr};
    } else if (signal == "blocked"){
        return {false, 0, 0};
    }

    // this line should never be reached
    throw "Error from InstrFetch::proceed(string signal): invalid signal: ";
    return {false, 0, 0};

}
