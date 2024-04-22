#pragma once
#ifndef I_LOAD_H
#define I_LOAD_H

#include "Instruction.h"

class I_Load : public Instruction {
public:
    I_Load(const Instr_Name& instr_identifier, const Instr_Type& instr_type,
           unsigned int rd_index, unsigned int rs1_index, int imm, bool pending_status_of_src_reg = false);
    I_Load(Instr_Name&& instr_identifier, Instr_Type&& instr_type,
           unsigned int rd_index, unsigned int rs1_index, int imm, bool pending_status_of_src_reg = false);
    ~I_Load();

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

    unsigned int get_addr();

    map<string, unsigned int> get_member_map();

private:
    unsigned int rd_index;
    unsigned int rd_value;
    unsigned int rs1_index;
    unsigned int rs1_value;
    int imm;
    bool pending_status_of_src_reg;
    unsigned int addr;
};

#endif // I_LOAD_H
