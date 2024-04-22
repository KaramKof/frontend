#ifndef HALT_H
#define HALT_H

#include "Instruction.h"

class HALT : public Instruction {
public:
    HALT(unsigned int pc_addr);  // Default constructor
    ~HALT(); // Default destructor

    void set_pc_addr(unsigned int pc_addr);

    unsigned int get_pc_addr();

private:
    unsigned int pc_addr;
};

#endif // HALT_H