#ifndef PROGRAM_COUNTER_H
#define PROGRAM_COUNTER_H

class ProgramCounter {
public:
    ProgramCounter();

    // Other class members, variables, and methods go here
    unsigned int get_current_instruction_address();

    unsigned int get_previous_instruction_address();

    void reset_instruction_address();

    void increment_instruction_address_by_a_word();

    void set_instruction_address_to(unsigned int address);

private:
    unsigned int instruction_address;;
    unsigned int prev_instr_addr;
};

#endif // PROGRAM_COUNTER_H
