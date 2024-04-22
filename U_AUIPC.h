#pragma once
#ifndef U_AUIPC_H
#define U_AUIPC_H

#include "Instruction.h"

class U_AUIPC : public Instruction {
public:
    U_AUIPC(const Instr_Name& instr_identifier, const Instr_Type& instr_type,
            unsigned int rd_index, int imm, unsigned int pc_addr );
    U_AUIPC(Instr_Name&& instr_identifier, Instr_Type&& instr_type,
            unsigned int rd_index, int imm, unsigned int pc_addr );
    ~U_AUIPC();

    unsigned int get_rd_index();

    void set_rd_index(unsigned int rd_index);

    unsigned int get_rd_value();

    void set_rd_value(unsigned int rd_value);

    void set_immediate_value(int imm);

    int get_immediate_value();

    void set_pc_addr(unsigned int pc_addr);

    unsigned int get_pc_addr();

    void execute();

    map<string, unsigned int> get_member_map();

private:
    unsigned int rd_index;
    unsigned int rd_value;
    int imm;
    unsigned int pc_addr;

};

#endif // U_AUIPC_H
