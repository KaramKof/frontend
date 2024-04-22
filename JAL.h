#pragma once
#ifndef JAL_H
#define JAL_H

#include "Instruction.h"

class JAL : public Instruction {
public:
    JAL(const Instr_Name& instr_identifier, const Instr_Type& instr_type,
        unsigned int rd_index, int imm, unsigned int pc_addr, bool pending_status_of_rd = false);
    JAL(Instr_Name&& instr_identifier, Instr_Type&& instr_type,
        unsigned int rd_index, int imm, unsigned int pc_addr, bool pending_status_of_rd = false);
    ~JAL();

    unsigned int get_rd_index();

    void set_rd_index(unsigned int rd_index);

    unsigned int get_rd_value();

    void set_rd_value(unsigned int rd_value);

    void set_immediate_value(int imm);

    int get_immediate_value();

    void set_pending_status_of_rd(bool pending_status_of_rd);

    void set_pc_addr(unsigned int pc_addr);

    unsigned int get_pc_addr();

    void execute();

    map<string, unsigned int> get_member_map();

private:
    unsigned int rd_index;
    unsigned int rd_value;
    int imm;
    unsigned int pc_addr;
    bool pending_status_of_rd;
};

#endif // JAL_H
