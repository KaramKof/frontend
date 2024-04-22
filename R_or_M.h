#pragma once
#ifndef R_OR_M_H
#define R_OR_M_H

#include "Instruction.h"


class R_or_M : public Instruction {
public:
    R_or_M(const Instr_Name& instr_identifier, const Instr_Type& instr_type,
           unsigned int rd_index, unsigned int rs1_index, unsigned int rs2_index,
           bool pending_status_of_src_reg = false);
    R_or_M(Instr_Name&& instr_identifier, Instr_Type&& instr_type,
           unsigned int rd_index, unsigned int rs1_index, unsigned int rs2_index,
           bool pending_status_of_src_reg = false);  // Constructor
    ~R_or_M(); // Destructor

    unsigned int get_rs1_index();

    unsigned int get_rs2_index();

    unsigned int get_rd_index();

    void set_rs1_value(unsigned int rs1_value);

    void set_rs2_value(unsigned int rs2_value);

    void set_rd_value(unsigned int rd_value);

    unsigned int get_rs1_value();

    unsigned int get_rs2_value();

    unsigned int get_rd_value();

    void set_rs1_index(unsigned int rs1_index);

    void set_rs2_index(unsigned int rs2_index);

    void set_rd_index(unsigned int rd_index);

    void set_pending_status_of_src_reg(bool pending_status_of_src_reg);

    void execute();

    map<string, unsigned int> get_member_map();

private:
    unsigned int rs1_index;
    unsigned int rs2_index;
    unsigned int rd_index;
    unsigned int rs1_value;
    unsigned int rs2_value;
    unsigned int rd_value;
    bool pending_status_of_src_reg;
};

#endif // R_OR_M_H
