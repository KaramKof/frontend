#include "HALT.h"

HALT::HALT(unsigned int pc_addr) : Instruction(Instr_Name::HALT, Instr_Type::HALT), pc_addr(pc_addr) {}

HALT::~HALT() {}

void HALT::set_pc_addr(unsigned int pc_addr) {
    this->pc_addr = pc_addr;
}

unsigned int HALT::get_pc_addr() {
    return pc_addr;
}