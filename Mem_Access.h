#pragma once
#ifndef MEM_ACCESS_H
#define MEM_ACCESS_H

#include "Execution.h"

class MemAccess {
public:
    MemAccess(Cache& data_cache, Execution& execution);
    ~MemAccess();

    void reset();

    Instruction* get_next_instr();

    Instruction* proceed(string signal);
    // Add your methods here
private:
    Cache& data_cache;
    Execution& execution;
    // the instruction returned from Execution, could be a STALL
    Instruction* saved_instr;
    // on_hold_instr from last cycle
    Instruction* on_hold_instr;
    // indicates if Execution has done processing the previous cycle's instruction
    bool is_last_cycle_instr_finished;
    // indicates if previous cycle's instruction has been passed on to the next stage
    bool is_last_cycle_instr_passed_over;

};


#endif // MEM_ACCESS_H
