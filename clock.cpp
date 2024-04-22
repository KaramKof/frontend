#include "Clock.h"

Clock::Clock(WriteBack& write_back, ProgramCounter& pc)
    : write_back(write_back), program_counter(pc) , cycle_count(0), cycles_to_run(0) {

}

Clock::~Clock() {
    // Destructor
}

void Clock::reset(){
    write_back.reset();
    cycle_count = 0;
    cycles_to_run = 0;
}

Instruction* Clock::run_one_cycle() {
    cycle_count++;
    return write_back.proceed("permitted");

}

bool Clock::run_to_end(){
    Instruction* an_instr = nullptr;
    do{
        if(an_instr){
            delete an_instr;
            an_instr = nullptr;
        }
        Instruction* an_instr = run_one_cycle();
        //cout << format("Cycle: {}\n", get_cycle_count());
    } while(an_instr->get_instr_name() != Instr_Name::HALT);

    return true;
}

void Clock::set_program_counter_to(unsigned int address){
    program_counter.set_instruction_address_to(address);
}

unsigned int Clock::get_cycle_count(){
    return cycle_count;
}
