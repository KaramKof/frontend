#pragma once
#ifndef EXECUTION_H
#define EXECUTION_H

#include "Instr_Decode.h"

class Execution {
public:
    Execution(ProgramCounter& pc, InstrDecode& instr_decode);
    ~Execution();

    void reset();

    Instruction* proceed(string signal);

    Instruction* get_next_instr();

private:
    ProgramCounter& program_counter;
    InstrDecode& instr_decode;
    // the instruction returned from InstrDecode, could be a STALL
    Instruction* saved_instr;
    // on_hold_instr from last cycle
    Instruction* on_hold_instr;
    // indicates if Execution has done processing the previous cycle's instruction
    bool is_last_cycle_instr_finished;
    // indicates if previous cycle's instruction has been passed on to the next stage
    bool is_last_cycle_instr_passed_over;
};

#endif // EXECUTION_H
