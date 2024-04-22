#pragma once
#ifndef S_H
#define S_H

#include "Instruction.h"

class S : public Instruction {
public:
    S(const Instr_Name& instr_identifier, const Instr_Type& instr_type,
      unsigned int rs1_index, unsigned int rs2_index, int imm, bool pending_status_of_src_reg = false);
    S(Instr_Name&& instr_identifier, Instr_Type&& instr_type,
      unsigned int rs1_index, unsigned int rs2_index, int imm, bool pending_status_of_src_reg = false);
    ~S();

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

    void set_pending_status_of_src_reg(bool pending_status_of_src_reg);

    unsigned int get_addr();

    void execute();

    map<string, unsigned int> get_member_map();

private:
    unsigned int rs1_index;
    unsigned int rs2_index;
    unsigned int rs1_value;
    unsigned int rs2_value;
    int imm;
    bool pending_status_of_src_reg;
    unsigned int addr;
};

#endif // S_H
