#include "program_counter.h"



ProgramCounter::ProgramCounter() : instruction_address(0), prev_instr_addr(0){

}

unsigned int ProgramCounter::get_current_instruction_address(){
    return instruction_address;
}

unsigned int ProgramCounter::get_previous_instruction_address(){
    return prev_instr_addr;
}

void ProgramCounter::reset_instruction_address(){
    instruction_address = 0;
    prev_instr_addr = 0;
    return;
}

void ProgramCounter::increment_instruction_address_by_a_word(){
    prev_instr_addr = instruction_address;
    instruction_address += 4;
    return;
}

void ProgramCounter::set_instruction_address_to(unsigned int address){
    prev_instr_addr = instruction_address;
    instruction_address = address;
    return;
}
