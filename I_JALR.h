#ifndef I_JALR_H
#define I_JALR_H

#include "Instruction.h"

class I_JALR : public Instruction {
public:
    I_JALR(const Instr_Name& instr_identifier, const Instr_Type& instr_type,
           unsigned int rd_index, unsigned int rs1_index, int imm, unsigned int pc_addr, bool pending_status_of_rs1_rd = false);
    I_JALR(Instr_Name&& instr_identifier, Instr_Type&& instr_type,
           unsigned int rd_index, unsigned int rs1_index, int imm, unsigned int pc_addr, bool pending_status_of_rs1_rd = false);
    ~I_JALR();

    unsigned int get_rd_index();

    void set_rd_index(unsigned int rd_index);

    unsigned int get_rd_value();

    void set_rd_value(unsigned int rd_value);

    unsigned int get_rs1_index();

    void set_rs1_index(unsigned int rs1_index);

    unsigned int get_rs1_value();

    void set_rs1_value(unsigned int rs1_value);

    void set_immediate_value(int imm);

    int get_immediate_value();

    void set_pending_status_of_rs1_rd(bool pending_status_of_rs1_rd);

    void set_pc_addr(unsigned int pc_addr);

    unsigned int get_pc_addr();

    void execute();

    map<string, unsigned int> get_member_map();

private:
    unsigned int rd_index;
    unsigned int rd_value;
    unsigned int rs1_index;
    unsigned int rs1_value;
    int imm;
    unsigned int pc_addr;
    bool pending_status_of_rs1_rd;
};

#endif // I_JALR_H
