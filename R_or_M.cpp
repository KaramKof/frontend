#include "R_or_M.h"

R_or_M::R_or_M(const Instr_Name &instr_identifier, const Instr_Type &instr_type,
               unsigned int rd_index, unsigned int rs1_index, unsigned int rs2_index,
               bool pending_status_of_src_reg)
    : Instruction(instr_identifier, instr_type), rd_index(rd_index), rs1_index(rs1_index), rs2_index(rs2_index),
    pending_status_of_src_reg(pending_status_of_src_reg), rd_value(0), rs1_value(0), rs2_value(0) {}


R_or_M::R_or_M(Instr_Name &&instr_identifier, Instr_Type &&instr_type,
               unsigned int rd_index, unsigned int rs1_index, unsigned int rs2_index,
               bool pending_status_of_src_reg)
    : R_or_M(instr_identifier, instr_type, rd_index, rs1_index, rs2_index, pending_status_of_src_reg) {}

unsigned int R_or_M::get_rs1_index() {
    return rs1_index;
}

R_or_M::~R_or_M(){}

unsigned int R_or_M::get_rs2_index() {
    return rs2_index;
}

unsigned int R_or_M::get_rd_index() {
    return rd_index;
}

void R_or_M::set_rs1_value(unsigned int rs1_value) {
    this->rs1_value = rs1_value;
}

void R_or_M::set_rs2_value(unsigned int rs2_value) {
    this->rs2_value = rs2_value;
}

void R_or_M::set_rd_value(unsigned int rd_value) {
    this->rd_value = rd_value;
}

unsigned int R_or_M::get_rs1_value() {
    return rs1_value;
}

unsigned int R_or_M::get_rs2_value() {
    return rs2_value;
}

unsigned int R_or_M::get_rd_value() {
    return rd_value;
}

void R_or_M::set_rs1_index(unsigned int rs1_index) {
    this->rs1_index = rs1_index;
}

void R_or_M::set_rs2_index(unsigned int rs2_index) {
    this->rs2_index = rs2_index;
}

void R_or_M::set_rd_index(unsigned int rd_index) {
    this->rd_index = rd_index;
}

void R_or_M::set_pending_status_of_src_reg(bool pending_status_of_src_reg) {
    this->pending_status_of_src_reg = pending_status_of_src_reg;
}

void R_or_M::execute(){
    // { // R extension
    // {0x00000000, Instr_Name::ADD },
    // {0x40000000, Instr_Name::SUB },
    // {0x00007000, Instr_Name::AND },
    // {0x00006000, Instr_Name::OR },
    // {0x00004000, Instr_Name::XOR },
    // {0x00005000, Instr_Name::SRL },
    // {0x40005000, Instr_Name::SRA },
    // {0x00002000, Instr_Name::SLT },
    // {0x00003000, Instr_Name::SLTU },
    // {0x00001000, Instr_Name::SLL },
    // // M extension
    // {0x02000000, Instr_Name::MUL },
    // {0x02001000, Instr_Name::MULH },
    // {0x02003000, Instr_Name::MULHU },
    // {0x02004000, Instr_Name::DIV },
    // {0x02005000, Instr_Name::DIVU },
    // {0x02006000, Instr_Name::REM },
    // {0x02007000, Instr_Name::REMU }, }
    bool is_overflow = false;
    switch (instr_identifier)
    {
    case Instr_Name::ADD:
        rd_value = static_cast<int>(rs1_value) + static_cast<int>(rs2_value);
        break;
    case Instr_Name::SUB:
        rd_value = static_cast<int>(rs1_value) - static_cast<int>(rs2_value);
        break;
    case Instr_Name::AND:
        rd_value = rs1_value & rs2_value;
        break;
    case Instr_Name::OR:
        rd_value = rs1_value | rs2_value;
        break;
    case Instr_Name::XOR:
        rd_value = rs1_value ^ rs2_value;
        break;
    case Instr_Name::SLL:
        rd_value = rs1_value << rs2_value;
        break;
    case Instr_Name::SRL:
        rd_value = rs1_value >> rs2_value;
        break;
    case Instr_Name::SRA:
        rd_value = (static_cast<int>(rs1_value)) >> rs2_value;
        break;
    case Instr_Name::SLT:
        rd_value = (static_cast<int>(rs1_value) < static_cast<int>(rs2_value)) ? 1 : 0;
        break;
    case Instr_Name::SLTU:
        rd_value = (rs1_value < rs2_value) ? 1 : 0;
        break;
    case Instr_Name::MUL:
        // The __builtin_mul_overflow function is a built-in function provided by the GCC compiler.
        // It's used to perform multiplication and check for overflow at the same time.
        if(__builtin_mul_overflow(static_cast<int>(rs1_value), static_cast<int>(rs2_value), &rd_value)){
            // Handle overflow
            is_overflow = true;
            int rs1_value_signed = static_cast<int>(rs1_value);
            int rs2_value_signed = static_cast<int>(rs2_value);
            rd_value = static_cast<unsigned int>((static_cast<long long>(rs1_value_signed) * static_cast<long long>(rs2_value_signed))
                                                 & static_cast<unsigned long long>(UINT_MAX));
        }
        break;
    case Instr_Name::MULH:
        if(__builtin_mul_overflow(static_cast<int>(rs1_value), static_cast<int>(rs2_value), &rd_value)){
            // Handle overflow
            is_overflow = true;
            int rs1_value_signed = static_cast<int>(rs1_value);
            int rs2_value_signed = static_cast<int>(rs2_value);
            rd_value = static_cast<unsigned int>((static_cast<unsigned long long>
                                                  (static_cast<long long>(rs1_value_signed) * static_cast<long long>(rs2_value_signed))
                                                  ) >> 32 );
        }else{
            rd_value = (rd_value & 0x80000000)? UINT_MAX: 0x00000000;
        }
        break;
    case Instr_Name::MULHU:
        if(__builtin_mul_overflow(rs1_value, rs2_value, &rd_value)){
            // Handle overflow
            is_overflow = true;
            rd_value = static_cast<unsigned int>(
                (static_cast<unsigned long long>(rs1_value) * static_cast<unsigned long long>(rs2_value))
                >> 32);
        }else{
            rd_value = 0x00000000;
        }
        break;
    case Instr_Name::DIV:
        if(rs2_value == 0){
            // Handle division by zero
            rd_value = UINT_MAX;
        }else{
            rd_value = static_cast<int>(rs1_value) / static_cast<int>(rs2_value);
        }
        break;
    case Instr_Name::DIVU:
        if(rs2_value == 0){
            // Handle division by zero
            rd_value = UINT_MAX;
        }else{
            rd_value = rs1_value / rs2_value;
        }
        break;
    case Instr_Name::REM:
        if(rs2_value == 0){
            // Handle division by zero
            rd_value = rs1_value;
        }else{
            rd_value = static_cast<int>(rs1_value) % static_cast<int>(rs2_value);
        }
        break;
    case Instr_Name::REMU:
        if(rs2_value == 0){
            // Handle division by zero
            rd_value = rs1_value;
        }else{
            rd_value = rs1_value % rs2_value;
        }
        break;
    case Instr_Name::NO_OP:
        cout << "Invalid bits pattern is detected. Instr_Name::NO_OP in Instr_Type::R_or_M." << endl;
        break;

    }
}

map<string, unsigned int> R_or_M::get_member_map(){
    map<string, unsigned int> member_map;
    member_map["rd_index"] = rd_index;
    member_map["rs1_index"] = rs1_index;
    member_map["rs2_index"] = rs2_index;
    member_map["rd_value"] = rd_value;
    member_map["rs1_value"] = rs1_value;
    member_map["rs2_value"] = rs2_value;
    return member_map;
}
