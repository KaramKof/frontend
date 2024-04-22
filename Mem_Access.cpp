#include "Mem_Access.h"


MemAccess::MemAccess(Cache& data_cache, Execution& execution)
    : data_cache(data_cache), execution(execution), saved_instr(new STALL()), on_hold_instr(nullptr),
    is_last_cycle_instr_finished(true), is_last_cycle_instr_passed_over(true) {
    // Constructor body
}

MemAccess::~MemAccess() {
    // Destructor body
}

void MemAccess::reset() {
    if(saved_instr) {
        delete saved_instr;
        saved_instr = new STALL();
    }

    if(on_hold_instr){
        delete on_hold_instr;
        on_hold_instr = nullptr;
    }

    is_last_cycle_instr_finished = true;
    is_last_cycle_instr_passed_over = true;
    execution.reset();
}

Instruction* MemAccess::get_next_instr(){
    if(on_hold_instr){
        return on_hold_instr;
    }
    return saved_instr;
}


Instruction* MemAccess::proceed(string signal) {
    Instruction* an_instr_obj;
    if(!is_last_cycle_instr_finished){
        an_instr_obj = on_hold_instr;
        Instr_Type instr_type = an_instr_obj->get_instr_type();
        switch (instr_type) {
        case Instr_Type::I_Load:
        {
            I_Load* load_instr = static_cast<I_Load*>(an_instr_obj);
            unsigned int addr = load_instr->get_addr();
            auto&& [response, word] = data_cache.read_a_word(addr);
            if(response){
                load_instr->set_rd_value(word);
                break;
            }
            // response is false, cache read is yet to be done
            is_last_cycle_instr_finished = false;
            is_last_cycle_instr_passed_over = false;
            saved_instr = execution.proceed("blocked");
            return new STALL();
        }

        case Instr_Type::S:
        {
            S* store_instr = static_cast<S*>(an_instr_obj);
            unsigned int addr = store_instr->get_addr();
            unsigned int word = store_instr->get_rs2_value();
            bool response = data_cache.write_a_word(addr, word);
            if(response){
                break;
            }
            // response is false, cache write is yet to be done
            is_last_cycle_instr_finished = false;
            is_last_cycle_instr_passed_over = false;
            saved_instr = execution.proceed("blocked");
            return new STALL();
        }

        }

    }else if(!is_last_cycle_instr_passed_over){
        an_instr_obj = on_hold_instr;

    }else { // last cycle instr is finished and passed over
        an_instr_obj = saved_instr;
        Instr_Type instr_type = an_instr_obj->get_instr_type();
        switch (instr_type){
        case Instr_Type::STALL:
            saved_instr = execution.proceed("permitted");
            return an_instr_obj;
        case Instr_Type::I_Load:
        {
            I_Load* load_instr = static_cast<I_Load*>(an_instr_obj);
            unsigned int addr = load_instr->get_addr();
            auto&& [response, word] = data_cache.read_a_word(addr);
            if(response){
                load_instr->set_rd_value(word);
                break;
            }
            // response is false, cache read is yet to be done
            on_hold_instr = load_instr;
            is_last_cycle_instr_finished = false;
            is_last_cycle_instr_passed_over = false;
            saved_instr = execution.proceed("blocked");
            return new STALL();
        }
        case Instr_Type::S:
        {
            S* store_instr = static_cast<S*>(an_instr_obj);
            unsigned int addr = store_instr->get_addr();
            unsigned int word = store_instr->get_rs2_value();
            bool response = data_cache.write_a_word(addr, word);
            if(response){
                break;
            }
            // response is false, cache write is yet to be done
            on_hold_instr = store_instr;
            is_last_cycle_instr_finished = false;
            is_last_cycle_instr_passed_over = false;
            saved_instr = execution.proceed("blocked");
            return new STALL();
        }

        }

    }

    // instr_type is either not from { STALL, I_Load, S },
    // or is I_Load/S but cache read/write is successfully done
    if(signal == "permitted"){
        on_hold_instr = nullptr;
        is_last_cycle_instr_finished = true;
        is_last_cycle_instr_passed_over = true;
        saved_instr = execution.proceed(signal);
        return an_instr_obj;
    }else if(signal == "blocked"){
        on_hold_instr = an_instr_obj;
        is_last_cycle_instr_finished = true;
        is_last_cycle_instr_passed_over = false;
        saved_instr = execution.proceed(signal);
        return new STALL();
    }

    // should not reach here
    throw "Error from MemAccess::proceed(string signal): invalid signal";

}
