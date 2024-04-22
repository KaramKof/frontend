#include "instruction.h"



Instruction::Instruction(const Instr_Name& instr_identifier, const Instr_Type& instr_type)
    : instr_identifier(instr_identifier), instr_type(instr_type) {}

Instruction::Instruction(Instr_Name&& instr_identifier, Instr_Type&& instr_type)
    : Instruction(instr_identifier, instr_type) {}

Instruction::~Instruction() {}

Instr_Name Instruction::get_instr_name() {
    return instr_identifier;
}

Instr_Type Instruction::get_instr_type() {
    return instr_type;
}

void Instruction::set_instr_name(Instr_Name& instr_identifier) {
    this->instr_identifier = instr_identifier;
}

void Instruction::set_instr_type(Instr_Type& instr_type) {
    this->instr_type = instr_type;
}

unsigned int Instruction::get_rd_index() {
    return 32;
}

unsigned int Instruction::get_rd_value() {
    return UINT_MAX;
}

void Instruction::execute(){
    // Do nothing
}

string Instruction::convert_instr_name_to_string() {
    switch (instr_identifier) {
    case Instr_Name::BEQ:
        return "Instr_Name::BEQ";
    case Instr_Name::BNE:
        return "Instr_Name::BNE";
    case Instr_Name::BLT:
        return "Instr_Name::BLT";
    case Instr_Name::BLTU:
        return "Instr_Name::BLTU";
    case Instr_Name::BGE:
        return "Instr_Name::BGE";
    case Instr_Name::BGEU:
        return "Instr_Name::BGEU";
    case Instr_Name::ADD:
        return "Instr_Name::ADD";
    case Instr_Name::SUB:
        return "Instr_Name::SUB";
    case Instr_Name::AND:
        return "Instr_Name::AND";
    case Instr_Name::OR:
        return "Instr_Name::OR";
    case Instr_Name::XOR:
        return "Instr_Name::XOR";
    case Instr_Name::SLL:
        return "Instr_Name::SLL";
    case Instr_Name::SRL:
        return "Instr_Name::SRL";
    case Instr_Name::SRA:
        return "Instr_Name::SRA";
    case Instr_Name::SLT:
        return "Instr_Name::SLT";
    case Instr_Name::SLTU:
        return "Instr_Name::SLTU";
    case Instr_Name::ADDI:
        return "Instr_Name::ADDI";
    case Instr_Name::ANDI:
        return "Instr_Name::ANDI";
    case Instr_Name::ORI:
        return "Instr_Name::ORI";
    case Instr_Name::XORI:
        return "Instr_Name::XORI";
    case Instr_Name::SLLI:
        return "Instr_Name::SLLI";
    case Instr_Name::SRLI:
        return "Instr_Name::SRLI";
    case Instr_Name::SRAI:
        return "Instr_Name::SRAI";
    case Instr_Name::SLTI:
        return "Instr_Name::SLTI";
    case Instr_Name::SLTIU:
        return "Instr_Name::SLTIU";
    case Instr_Name::MUL:
        return "Instr_Name::MUL";
    case Instr_Name::MULH:
        return "Instr_Name::MULH";
    case Instr_Name::MULHU:
        return "Instr_Name::MULHU";
    case Instr_Name::REM:
        return "Instr_Name::REM";
    case Instr_Name::REMU:
        return "Instr_Name::REMU";
    case Instr_Name::LW:
        return "Instr_Name::LW";
    case Instr_Name::SW:
        return "Instr_Name::SW";
    case Instr_Name::JAL:
        return "Instr_Name::JAL";
    case Instr_Name::JALR:
        return "Instr_Name::JALR";
    case Instr_Name::AUIPC:
        return "Instr_Name::AUIPC";
    case Instr_Name::LUI:
        return "Instr_Name::LUI";
    case Instr_Name::NO_OP:
        return "Instr_Name::NO_OP";
    case Instr_Name::STALL:
        return "Instr_Name::STALL";
    case Instr_Name::HALT:
        return "Instr_Name::HALT";
    default:
        return "Unknown Instr Name";
    }

}

map<string, unsigned int> Instruction::get_member_map(){
    return map<string, unsigned int>{};
}
