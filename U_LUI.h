#pragma once
#ifndef U_LUI_H
#define U_LUI_H

#include "Instruction.h"

class U_LUI : public Instruction {
public:
    U_LUI(const Instr_Name& instr_identifier, const Instr_Type& instr_type,
          unsigned int rd_index, int imm);
    U_LUI(Instr_Name&& instr_identifier, Instr_Type&& instr_type,
          unsigned int rd_index, int imm);
    ~U_LUI();

    unsigned int get_rd_index();

    void set_rd_index(unsigned int rd_index);

    unsigned int get_rd_value();

    void set_rd_value(unsigned int rd_value);

    void set_immediate_value(int imm);

    int get_immediate_value();

    void execute();

    map<string, unsigned int> get_member_map();

private:
    unsigned int rd_index;
    unsigned int rd_value;
    int imm;
};

#endif // U_LUI_H
