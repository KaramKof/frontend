#pragma once
#ifndef B_H
#define B_H

#include "Instruction.h"

class B : public Instruction {
public:
    B(const Instr_Name& instr_identifier, const Instr_Type& instr_type,
      unsigned int rs1_index, unsigned int rs2_index, int imm, unsigned int pc_addr, bool pending_status_of_src_reg = false);
    B(Instr_Name&& instr_identifier, Instr_Type&& instr_type,
      unsigned int rs1_index, unsigned int rs2_index, int imm, unsigned int pc_addr, bool pending_status_of_src_reg = false);
    ~B();

    unsigned int get_rs1_index();

    unsigned int get_rs2_index();

    void set_rs1_index(unsigned int rs1_index);

    void set_rs2_index(unsigned int rs2_index);

    unsigned int get_rs1_value();

    unsigned int get_rs2_value();

    void set_rs1_value(unsigned int rs1_value);

    void set_rs2_value(unsigned int rs2_value);

    void set_immediate_value(int imm);

    int get_immediate_value();

    bool is_any_src_reg_pending_write();

    void set_pending_status_of_src_reg(bool pending_status_of_src_reg);

    void set_pc_addr(unsigned int pc_addr);

    unsigned int get_pc_addr();

    void execute();

    bool is_branch_taken();

    map<string, unsigned int> get_member_map();

private:
    unsigned int rs1_index;
    unsigned int rs2_index;
    unsigned int rs1_value;
    unsigned int rs2_value;
    int imm;
    unsigned int pc_addr;
    bool pending_status_of_src_reg;
};

#endif // B_H
