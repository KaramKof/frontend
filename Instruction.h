#pragma once
#ifndef INSTR_H
#define INSTR_H

#include <string>
#include <iostream>
#include <map>

using std::string, std::cout, std::endl, std::map;


enum class Instr_Type{
    R_or_M,
    I_Basic,
    I_Load,
    S,
    B,
    JAL,
    I_JALR,
    U_AUIPC,
    U_LUI,
    NO_OP,
    STALL,
    HALT,
};

enum class Instr_Name {
    // From map_for_R_or_M_type
    // R extension
    ADD,
    SUB,
    AND,
    OR,
    XOR,
    SRL,
    SRA,
    SLT,
    SLTU,
    SLL,
    // M extension
    MUL,
    MULH,
    MULHU,
    DIV,
    DIVU,
    REM,
    REMU,

    // From map_for_I_Basic_type
    ADDI,
    ANDI,
    ORI,
    XORI,
    SLLI,
    SRLI,
    SRAI,
    SLTI,
    SLTIU,

    // From map_for_I_Load_type
    LW,

    // From map_for_S_type
    SW,

    // From map_for_B_type
    BEQ,
    BNE,
    BLT,
    BGE,
    BLTU,
    BGEU,

    // From map_for_JAL_type
    JAL,

    // From map_for_I_JALR_type
    JALR,

    // From map_for_U_AUIPC_type
    AUIPC,

    // From map_for_U_LUI_type
    LUI,
    // NO_OP
    NO_OP,
    // STALL
    STALL,
    // HALT
    HALT,

};


class Instruction{
public:

    Instruction(const Instr_Name& instr_identifier, const Instr_Type& instr_type);

    Instruction(Instr_Name&& instr_identifier, Instr_Type&& instr_type);
    virtual ~Instruction() = 0;

    Instr_Name get_instr_name();

    Instr_Type get_instr_type();

    void set_instr_name(Instr_Name& instr_identifier);

    void set_instr_type(Instr_Type& instr_type);

    virtual unsigned int get_rd_index();

    virtual unsigned int get_rd_value();

    virtual void execute();

    string convert_instr_name_to_string();

    virtual map<string, unsigned int> get_member_map();

protected:
    Instr_Name instr_identifier;
    Instr_Type instr_type;
};

#endif // INSTR_H
