#include "I_Basic.h"

I_Basic::I_Basic(const Instr_Name& instr_identifier, const Instr_Type& instr_type, unsigned int rd_index,
                 unsigned int rs1_index, int imm, bool pending_status_of_src_reg)
    : Instruction(instr_identifier, instr_type), rd_index(rd_index), rs1_index(rs1_index), imm(imm),
    pending_status_of_src_reg(pending_status_of_src_reg), rs1_value(0), rd_value(0) {}

I_Basic::I_Basic(Instr_Name&& instr_identifier, Instr_Type&& instr_type, unsigned int rd_index, unsigned int rs1_index,
                 int imm, bool pending_status_of_src_reg)
    : Instruction(std::move(instr_identifier), std::move(instr_type)), rd_index(rd_index), rs1_index(rs1_index),
    imm(imm), pending_status_of_src_reg(pending_status_of_src_reg), rs1_value(0), rd_value(0) {}

I_Basic::~I_Basic() {}

unsigned int I_Basic::get_rd_index() {
    return rd_index;
}

unsigned int I_Basic::get_rs1_index() {
    return rs1_index;
}

void I_Basic::set_rd_index(unsigned int rd_index) {
    this->rd_index = rd_index;
}

void I_Basic::set_rs1_index(unsigned int rs1_index) {
    this->rs1_index = rs1_index;
}

unsigned int I_Basic::get_rd_value() {
    return rd_value;
}

unsigned int I_Basic::get_rs1_value() {
    return rs1_value;
}

void I_Basic::set_rd_value(unsigned int rd_value) {
    this->rd_value = rd_value;
}

void I_Basic::set_rs1_value(unsigned int rs1_value) {
    this->rs1_value = rs1_value;
}

void I_Basic::set_immediate_value(int imm) {
    this->imm = imm;
}

int I_Basic::get_immediate_value() {
    return imm;
}

void I_Basic::set_pending_status_of_src_reg(bool pending_status_of_src_reg) {
    this->pending_status_of_src_reg = pending_status_of_src_reg;
}

void I_Basic::execute() {
    // {
    // {0x00000000, Instr_Name::ADDI },
    // {0x00007000, Instr_Name::ANDI },
    // {0x00006000, Instr_Name::ORI },
    // {0x00004000, Instr_Name::XORI },
    // {0x00002000, Instr_Name::SLTI },
    // {0x00003000, Instr_Name::SLTIU },
    // {0x00001000, Instr_Name::SLLI },
    // {0x00005000, Instr_Name::SRLI },
    // {0x40005000, Instr_Name::SRAI }, };
    switch (instr_identifier)
    {
    case Instr_Name::ADDI:
        rd_value = static_cast<int>(rs1_value) + imm;
        break;
    case Instr_Name::ANDI:
        rd_value = static_cast<int>(rs1_value) & imm;
        break;
    case Instr_Name::ORI:
        rd_value = static_cast<int>(rs1_value) | imm;
        break;
    case Instr_Name::XORI:
        rd_value = static_cast<int>(rs1_value) ^ imm;
        break;
    case Instr_Name::SLTI:
        rd_value = static_cast<int>(rs1_value) < imm ? 1 : 0;
        break;
    case Instr_Name::SLTIU:
        rd_value = rs1_value < imm ? 1 : 0;
        break;
    case Instr_Name::SLLI:
        rd_value = static_cast<int>(rs1_value) << static_cast<unsigned int>(imm);
        break;
    case Instr_Name::SRLI:
        rd_value = rs1_value >> static_cast<unsigned int>(imm);
        break;
    case Instr_Name::SRAI:
        rd_value = static_cast<int>(rs1_value) >> static_cast<unsigned int>(imm);
        break;
    case Instr_Name::NO_OP:
        cout << "Invalid bits pattern is detected. Instr_Name::NO_OP in Instr_Type::I_Basic." << endl;
        break;
    }

}


map<string, unsigned int> I_Basic::get_member_map(){
    return map<string, unsigned int> {{"rs1_index",rs1_index}, {"rs1_value", rs1_value},
        {"rd_index", rd_index}, {"rd_value", rd_value }, {"imm", imm}, };

}
