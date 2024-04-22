#ifndef CLOCK_H
#define CLOCK_H

#include "Write_Back.h"

class Clock {
public:
    Clock(WriteBack& write_back, ProgramCounter& pc);  // Default constructor
    ~Clock(); // Default destructor

    Instruction* run_one_cycle();

    bool run_to_end();

    void set_program_counter_to(unsigned int address);

    void reset();

    unsigned int get_cycle_count();


private:
    unsigned int cycle_count;
    unsigned int cycles_to_run;
    WriteBack& write_back;
    ProgramCounter& program_counter;
};

#endif // CLOCK_H
