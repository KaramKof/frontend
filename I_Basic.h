#pragma once
#ifndef I_BASIC_H
#define I_BASIC_H

#include "Instruction.h"

class I_Basic : public Instruction {
public:
    I_Basic(const Instr_Name& instr_identifier, const Instr_Type& instr_type,
            unsigned int rd_index, unsigned int rs1_index, int imm, bool pending_status_of_src_reg = false);
    I_Basic(Instr_Name&& instr_identifier, Instr_Type&& instr_type,
            unsigned int rd_index, unsigned int rs1_index, int imm, bool pending_status_of_src_reg = false);
    ~I_Basic();

    unsigned int get_rd_index();

    unsigned int get_rs1_index();

    void set_rd_index(unsigned int rd_index);

    void set_rs1_index(unsigned int rs1_index);

    unsigned int get_rd_value();

    unsigned int get_rs1_value();

    void set_rd_value(unsigned int rd_value);

    void set_rs1_value(unsigned int rs1_value);

    void set_immediate_value(int imm);

    int get_immediate_value();

    void set_pending_status_of_src_reg(bool pending_status_of_src_reg);

    void execute();

    map<string, unsigned int> get_member_map();

private:
    unsigned int rd_index;
    unsigned int rd_value;
    unsigned int rs1_index;
    unsigned int rs1_value;
    int imm;
    bool pending_status_of_src_reg;

};

#endif // I_BASIC_H
