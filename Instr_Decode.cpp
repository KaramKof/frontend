
#include "Instr_Decode.h"



InstrDecode::InstrDecode(RegisterFile& register_file, InstrFetch& instr_fetch)
    : register_file(register_file), instr_fetch(instr_fetch), saved_instr({false, 0, 0}),
    on_hold_instr(nullptr), is_last_cycle_instr_finished(true), is_last_cycle_instr_passed_over(true),
    map_of_opcode_to_instr_type ({
                                 {0b0110011, Instr_Type::R_or_M },
                                 {0b0010011, Instr_Type::I_Basic },
                                 {0b0000011, Instr_Type::I_Load },
                                 {0b0100011, Instr_Type::S },
                                 {0b1100011, Instr_Type::B },
                                 {0b1101111, Instr_Type::JAL },
                                 {0b1100111, Instr_Type::I_JALR },
                                 {0b0010111, Instr_Type::U_AUIPC },
                                 {0b0110111, Instr_Type::U_LUI },
                                 {0b0000000, Instr_Type::HALT } }),
    map_for_R_or_M_type ({ // R extension
        {0x00000000, Instr_Name::ADD },
        {0x40000000, Instr_Name::SUB },
        {0x00007000, Instr_Name::AND },
        {0x00006000, Instr_Name::OR },
        {0x00004000, Instr_Name::XOR },
        {0x00001000, Instr_Name::SLL },
        {0x00005000, Instr_Name::SRL },
        {0x40005000, Instr_Name::SRA },
        {0x00002000, Instr_Name::SLT },
        {0x00003000, Instr_Name::SLTU },
        // M extension
        {0x02000000, Instr_Name::MUL },
        {0x02001000, Instr_Name::MULH },
        {0x02003000, Instr_Name::MULHU },
        {0x02004000, Instr_Name::DIV },
        {0x02005000, Instr_Name::DIVU },
        {0x02006000, Instr_Name::REM },
        {0x02007000, Instr_Name::REMU }, }),

    map_for_I_Basic_type ({
        {0x00000000, Instr_Name::ADDI },
        {0x00007000, Instr_Name::ANDI },
        {0x00006000, Instr_Name::ORI },
        {0x00004000, Instr_Name::XORI },
        {0x00002000, Instr_Name::SLTI },
        {0x00003000, Instr_Name::SLTIU },
        {0x00001000, Instr_Name::SLLI },
        {0x00005000, Instr_Name::SRLI },
        {0x40005000, Instr_Name::SRAI }, }),

    map_for_I_Load_type ({ {0x00002000, Instr_Name::LW } }),

    map_for_S_type ({ {0x00002000, Instr_Name::SW } }),

    map_for_B_type ({
        {0x00000000, Instr_Name::BEQ },
        {0x00001000, Instr_Name::BNE },
        {0x00005000, Instr_Name::BGE },
        {0x00007000, Instr_Name::BGEU },
        {0x00004000, Instr_Name::BLT },
        {0x00006000, Instr_Name::BLTU }, }),

    map_for_JAL_type ({ {0x00000000, Instr_Name::JAL }, }),

    map_for_I_JALR_type ({ {0x00000000, Instr_Name::JALR }, }),

    map_for_U_AUIPC_type ({ {0x00000000, Instr_Name::AUIPC }, }),

    map_for_U_LUI_type ({ {0x00000000, Instr_Name::LUI }, }),

    map_of_various_decode_funcs ({
                                 {Instr_Type::R_or_M, bind(&InstrDecode::decode_R_or_M_type, this, _1, _2)},
                                 {Instr_Type::I_Basic, bind(&InstrDecode::decode_I_Basic_type, this, _1, _2)},
                                 {Instr_Type::I_Load, bind(&InstrDecode::decode_I_Load_type, this, _1, _2)},
                                 {Instr_Type::S, bind(&InstrDecode::decode_S_type, this, _1, _2)},
                                 {Instr_Type::B, bind(&InstrDecode::decode_B_type, this, _1, _2)},
                                 {Instr_Type::JAL, bind(&InstrDecode::decode_JAL_type, this, _1, _2)},
                                 {Instr_Type::I_JALR, bind(&InstrDecode::decode_I_JALR_type, this, _1, _2)},
                                 {Instr_Type::U_AUIPC, bind(&InstrDecode::decode_U_AUIPC_type, this, _1, _2)},
                                 {Instr_Type::U_LUI, bind(&InstrDecode::decode_U_LUI_type, this, _1, _2)},
                                 {Instr_Type::HALT, bind(&InstrDecode::decode_HALT_type, this, _1, _2)},
    }),

    map_of_various_redecode_funcs ({
                                   {Instr_Type::R_or_M, bind(&InstrDecode::redecode_R_or_M_type, this, _1)},
                                   {Instr_Type::I_Basic, bind(&InstrDecode::redecode_I_Basic_type, this, _1)},
                                   {Instr_Type::I_Load, bind(&InstrDecode::redecode_I_Load_type, this, _1)},
                                   {Instr_Type::S, bind(&InstrDecode::redecode_S_type, this, _1)},
                                   {Instr_Type::B, bind(&InstrDecode::redecode_B_type, this, _1)},
                                   {Instr_Type::JAL, bind(&InstrDecode::redecode_JAL_type, this, _1)},
                                   {Instr_Type::I_JALR, bind(&InstrDecode::redecode_I_JALR_type, this, _1)},

                                   })
{}

InstrDecode::~InstrDecode(){
    if(on_hold_instr){
        delete on_hold_instr;
    }
}

void InstrDecode::reset(){
    if(on_hold_instr){
        delete on_hold_instr;
        on_hold_instr = nullptr;
    }
    saved_instr = {false, 0, 0};
    is_last_cycle_instr_finished = true;
    is_last_cycle_instr_passed_over = true;
    instr_fetch.reset();
}

tuple<bool, unsigned int, unsigned int> InstrDecode::get_saved_instr(){
    return saved_instr;
}

Instruction* InstrDecode::get_on_hold_instr(){
    return on_hold_instr;
}


// the 2nd element of tuple (boolean indicator) indicates if any source register(s) is/are pending write.
// returns true if any source register(s) is/are pending write, meaning the decode has to stall,
// otherwise returns false.
tuple<R_or_M*, bool> InstrDecode::decode_R_or_M_type(unsigned int instr_word, unsigned int instr_addr){
    unsigned int value_of_funct7_and_funct3 = instr_word & mask_for_funct7_and_funct3;
    unsigned int rd_index = ( instr_word >> num_of_bits_to_indicate_opcode) & mask_for_register_index;
    unsigned int rs1_index = ( instr_word >> (num_of_bits_to_indicate_opcode
                                             + num_of_bits_to_indicate_register_index
                                             + num_of_bits_to_indicate_funct3)) & mask_for_register_index;
    unsigned int rs2_index = ( instr_word >> (num_of_bits_to_indicate_opcode
                                             + num_of_bits_to_indicate_register_index
                                             + num_of_bits_to_indicate_funct3
                                             + num_of_bits_to_indicate_register_index)) & mask_for_register_index;
    auto iter = map_for_R_or_M_type.find(value_of_funct7_and_funct3);
    // bits pattern for funct7 and funct3 might not be valid/exist in the map
    Instr_Name identifier = (iter == map_for_R_or_M_type.end())? Instr_Name::NO_OP : iter->second;

    bool pending_status_of_src_reg = register_file.is_register_pending_write(rs1_index) ||
                                     register_file.is_register_pending_write(rs2_index);
    auto* a_instr_obj = new R_or_M(identifier, Instr_Type::R_or_M, rd_index, rs1_index, rs2_index, pending_status_of_src_reg);
    if(!pending_status_of_src_reg){ // neither rs1 nor rs2 are pending write
        a_instr_obj->set_rs1_value(register_file.read_a_word_by_reg_index(rs1_index));
        a_instr_obj->set_rs2_value(register_file.read_a_word_by_reg_index(rs2_index));
    }
    return {a_instr_obj, pending_status_of_src_reg};
}


// redecode the R or M type instruction because its source register(s) were pending write in last cycle.
// returns true if any source register(s) is/are still pending write, meaning the decode has to stall,
// otherwise returns false.
bool InstrDecode::redecode_R_or_M_type(Instruction* instr_obj){
    auto* instr_of_R_or_M_type = static_cast<R_or_M*>(instr_obj);
    unsigned int rs1_index = instr_of_R_or_M_type->get_rs1_index();
    unsigned int rs2_index = instr_of_R_or_M_type->get_rs2_index();
    bool pending_status_of_src_reg = register_file.is_register_pending_write(rs1_index) || register_file.is_register_pending_write(rs2_index);
    instr_of_R_or_M_type->set_pending_status_of_src_reg(pending_status_of_src_reg);
    if(!pending_status_of_src_reg){ // neither rs1 nor rs2 are pending write
        instr_of_R_or_M_type->set_rs1_value(register_file.read_a_word_by_reg_index(rs1_index));
        instr_of_R_or_M_type->set_rs2_value(register_file.read_a_word_by_reg_index(rs2_index));
    }
    return pending_status_of_src_reg;

}

tuple<I_Basic*, bool> InstrDecode::decode_I_Basic_type(unsigned int instr_word, unsigned int instr_addr){
    unsigned int value_of_funct3 = instr_word & mask_for_funct3;
    unsigned int value_of_funct3_or_funct7_func3 = value_of_funct3;
    // immdiates are signed extended
    int immediate = (static_cast<int>(instr_word)) >> (num_of_bits_to_indicate_opcode
                                                       + num_of_bits_to_indicate_register_index
                                                       + num_of_bits_to_indicate_funct3
                                                       + num_of_bits_to_indicate_register_index);
    // cout << "In decode_I_Basic_type immediate: " << immediate << __LINE__ << endl;
    if(value_of_funct3 == 0x00001000 || value_of_funct3 == 0x00005000){ // SLLI or SRLI or SRAI
        value_of_funct3_or_funct7_func3 = instr_word & mask_for_funct7_and_funct3;
        // immediates for SLLI, SRLI, SRAI are not signed extended
        immediate = static_cast<int>(( instr_word >> (num_of_bits_to_indicate_opcode
                                                     + num_of_bits_to_indicate_register_index
                                                     + num_of_bits_to_indicate_funct3
                                                     + num_of_bits_to_indicate_register_index)) & 0x1F);
    }
    unsigned int rd_index = ( instr_word >> num_of_bits_to_indicate_opcode) & mask_for_register_index;
    unsigned int rs1_index = ( instr_word >> (num_of_bits_to_indicate_opcode
                                             + num_of_bits_to_indicate_register_index
                                             + num_of_bits_to_indicate_funct3)) & mask_for_register_index;
    // cout << "In decode_I_Basic_type rd_index: " << rd_index << __LINE__ << endl;
    // cout << "In decode_I_Basic_type rs1_index: " << rs1_index << __LINE__ << endl;
    auto iter = map_for_I_Basic_type.find(value_of_funct3_or_funct7_func3);
    // bits pattern for funct3 or funct7 and funct3 might not be valid/exist in the map
    Instr_Name identifier = (iter == map_for_I_Basic_type.end())? Instr_Name::NO_OP : iter->second;

    bool pending_status_of_src_reg = register_file.is_register_pending_write(rs1_index);

    auto* a_instr_obj = new I_Basic(identifier, Instr_Type::I_Basic, rd_index, rs1_index, immediate,
                                    pending_status_of_src_reg);
    if(!pending_status_of_src_reg){ // rs1 is not pending write
        a_instr_obj->set_rs1_value(register_file.read_a_word_by_reg_index(rs1_index));
        // cout << "In decode_I_Basic_type inside if block rs1_value: " << a_instr_obj->get_rs1_value() << __LINE__ << endl;
    }
    // cout << "In decode_I_Basic_type outside if block rs1_value: " << a_instr_obj->get_rs1_value() << __LINE__ << endl;
    return {a_instr_obj, pending_status_of_src_reg};
}

bool InstrDecode::redecode_I_Basic_type(Instruction* instr_obj){
    auto* instr_of_I_Basic_type = static_cast<I_Basic*>(instr_obj);
    unsigned int rs1_index = instr_of_I_Basic_type->get_rs1_index();
    bool pending_status_of_src_reg = register_file.is_register_pending_write(rs1_index);
    instr_of_I_Basic_type->set_pending_status_of_src_reg(pending_status_of_src_reg);
    if(!pending_status_of_src_reg){ // rs1 is not pending write
        instr_of_I_Basic_type->set_rs1_value(register_file.read_a_word_by_reg_index(rs1_index));
        // cout << "In redecode_I_Basic_type inside if block rs1_value: " << instr_of_I_Basic_type->get_rs1_value() << endl;
    }
    return pending_status_of_src_reg;

}


tuple<I_Load*, bool> InstrDecode::decode_I_Load_type(unsigned int instr_word, unsigned int instr_addr){
    unsigned int value_of_funct3 = instr_word & mask_for_funct3;

    unsigned int rd_index = ( instr_word >> num_of_bits_to_indicate_opcode) & mask_for_register_index;
    // cout << "rd_index: " << rd_index << endl;
    unsigned int rs1_index = ( instr_word >> (num_of_bits_to_indicate_opcode
                                             + num_of_bits_to_indicate_register_index
                                             + num_of_bits_to_indicate_funct3)) & mask_for_register_index;
    // cout << "rs1_index: " << rs1_index << endl;
    // immdiates are signed extended
    int immediate = (static_cast<int>(instr_word)) >> (num_of_bits_to_indicate_opcode
                                                       + num_of_bits_to_indicate_register_index
                                                       + num_of_bits_to_indicate_funct3
                                                       + num_of_bits_to_indicate_register_index);
    // cout << "immediate: " << immediate << endl;
    auto iter = map_for_I_Load_type.find(value_of_funct3);
    // bits pattern for funct3 might not be valid/exist in the map
    Instr_Name identifier = (iter == map_for_I_Load_type.end())? Instr_Name::NO_OP : iter->second;

    bool pending_status_of_src_reg = register_file.is_register_pending_write(rs1_index);
    auto* a_instr_obj = new I_Load(identifier, Instr_Type::I_Load, rd_index, rs1_index, immediate,
                                   pending_status_of_src_reg);
    if(!pending_status_of_src_reg){ // rs1 is not pending write
        a_instr_obj->set_rs1_value(register_file.read_a_word_by_reg_index(rs1_index));
        // cout<< "In side decode_I_Load_type inside block: rs1_value: " << a_instr_obj->get_rs1_value() << endl;
    }
    // cout << "In side decode_I_Load_type outside if block rs1_value: " << a_instr_obj->get_rs1_value() << endl;
    return {a_instr_obj, pending_status_of_src_reg};
}

bool InstrDecode::redecode_I_Load_type(Instruction* instr_obj){
    auto* instr_of_I_Load_type = static_cast<I_Load*>(instr_obj);
    unsigned int rs1_index = instr_of_I_Load_type->get_rs1_index();
    bool pending_status_of_src_reg = register_file.is_register_pending_write(rs1_index);
    instr_of_I_Load_type->set_pending_status_of_src_reg(pending_status_of_src_reg);
    if(!pending_status_of_src_reg){ // rs1 is not pending write
        instr_of_I_Load_type->set_rs1_value(register_file.read_a_word_by_reg_index(rs1_index));
    }
    // cout << "rs1_value: " << instr_of_I_Load_type->get_rs1_value() << endl;
    return pending_status_of_src_reg;
}


tuple<S*, bool> InstrDecode::decode_S_type(unsigned int instr_word, unsigned int instr_addr) {
    unsigned int value_of_funct3 = instr_word & mask_for_funct3;

    unsigned int imm4to0 = (instr_word >> num_of_bits_to_indicate_opcode) & mask_for_register_index;
    unsigned int rs1_index = (instr_word >> (num_of_bits_to_indicate_opcode
                                             + num_of_bits_to_indicate_register_index
                                             + num_of_bits_to_indicate_funct3)) & mask_for_register_index;

    unsigned int rs2_index = (instr_word >> (num_of_bits_to_indicate_opcode
                                             + num_of_bits_to_indicate_register_index
                                             + num_of_bits_to_indicate_funct3
                                             + num_of_bits_to_indicate_register_index )) & mask_for_register_index;

    int imm11to5 = (static_cast<int>(instr_word & mask_for_funct7)) >> (num_of_bits_to_indicate_a_word
                                                                        - num_of_bits_to_indicate_funct7 - num_of_bits_to_indicate_register_index);
    int immediate = imm11to5 | imm4to0 ;

    bool pending_status_of_src_reg = register_file.is_register_pending_write(rs1_index) ||
                                     register_file.is_register_pending_write(rs2_index);

    auto iter = map_for_S_type.find(value_of_funct3);
    // bits pattern for funct3 might not be valid/exist in the map
    Instr_Name identifier = (iter == map_for_S_type.end())? Instr_Name::NO_OP : iter->second;

    auto* a_instr_obj = new S(identifier, Instr_Type::S, rs1_index, rs2_index, immediate,
                              pending_status_of_src_reg);
    if (!pending_status_of_src_reg) { // rs1 and rs2 are not pending write
        a_instr_obj->set_rs1_value(register_file.read_a_word_by_reg_index(rs1_index));
        a_instr_obj->set_rs2_value(register_file.read_a_word_by_reg_index(rs2_index));
    }
    return {a_instr_obj, pending_status_of_src_reg};

}

bool InstrDecode::redecode_S_type(Instruction* instr_obj) {
    auto* instr_of_S_type = static_cast<S*>(instr_obj);
    unsigned int rs1_index = instr_of_S_type->get_rs1_index();
    unsigned int rs2_index = instr_of_S_type->get_rs2_index();

    bool pending_status_of_src_reg = register_file.is_register_pending_write(rs1_index) ||
                                     register_file.is_register_pending_write(rs2_index);

    instr_of_S_type->set_pending_status_of_src_reg(pending_status_of_src_reg);

    if (!pending_status_of_src_reg) { // rs1 and rs2 are not pending write
        instr_of_S_type->set_rs1_value(register_file.read_a_word_by_reg_index(rs1_index));
        instr_of_S_type->set_rs2_value(register_file.read_a_word_by_reg_index(rs2_index));
    }

    return pending_status_of_src_reg;
}

tuple<B*, bool> InstrDecode::decode_B_type(unsigned int instr_word, unsigned int instr_addr) {
    unsigned int value_of_funct3 = instr_word & mask_for_funct3;

    unsigned int rs1_index = (instr_word >> (num_of_bits_to_indicate_opcode
                                             + num_of_bits_to_indicate_register_index
                                             + num_of_bits_to_indicate_funct3)) & mask_for_register_index;

    unsigned int rs2_index = (instr_word >> (num_of_bits_to_indicate_opcode
                                             + num_of_bits_to_indicate_register_index
                                             + num_of_bits_to_indicate_funct3
                                             + num_of_bits_to_indicate_register_index )) & mask_for_register_index;

    // least significant bit of immediate (imm[0]) is always 0 and thus left out in the imm fields of an branch instruction
    // precisely following the RISC-V spec
    // imm[12] = word[31]
    int imm12 = (static_cast<int>(instr_word) & (1 << (num_of_bits_to_indicate_a_word - 1))) >> (31 - 12);
    // imm[10:5] = word[30:25]
    unsigned int imm10to5 = (instr_word & mask_for_B_type_imm_10_to_5) >> (25 - 5);
    // imm[4:1] = word[11:8]
    unsigned int imm4to1 = (instr_word & mask_for_B_type_imm_4_to_1) >> (8 - 1);
    // imm[11] = word[7]
    unsigned int imm11 = (instr_word & mask_for_B_type_imm_11) << (11 - 7);
    int immediate = imm12 | imm11 | imm10to5 | imm4to1;

    // // initial plan of immediate representation imm12to6 and imm5to1
    // unsigned int imm5to1 = ((instr_word >> num_of_bits_to_indicate_opcode) & mask_for_register_index ) << 1;

    // int imm12to6 = (static_cast<int>(instr_word & mask_for_funct7)) >> (num_of_bits_to_indicate_a_word
    //                         - num_of_bits_to_indicate_funct7 - num_of_bits_to_indicate_register_index -1);

    // int immediate = imm12to6 | imm5to1;

    bool pending_status_of_src_reg = register_file.is_register_pending_write(rs1_index) ||
                                     register_file.is_register_pending_write(rs2_index);

    auto iter = map_for_B_type.find(value_of_funct3);
    // bits pattern for funct3 might not be valid/exist in the map
    Instr_Name identifier = (iter == map_for_B_type.end())? Instr_Name::NO_OP : iter->second;

    auto* a_instr_obj = new B(identifier, Instr_Type::B, rs1_index, rs2_index, immediate, instr_addr,
                              pending_status_of_src_reg);
    if (!pending_status_of_src_reg) { // rs1 and rs2 are not pending write
        a_instr_obj->set_rs1_value(register_file.read_a_word_by_reg_index(rs1_index));
        a_instr_obj->set_rs2_value(register_file.read_a_word_by_reg_index(rs2_index));
    }

    return {a_instr_obj, pending_status_of_src_reg};
}

bool InstrDecode::redecode_B_type(Instruction* instr_obj) {
    auto* instr_of_B_type = static_cast<B*>(instr_obj);
    unsigned int rs1_index = instr_of_B_type->get_rs1_index();
    unsigned int rs2_index = instr_of_B_type->get_rs2_index();

    bool pending_status_of_src_reg = register_file.is_register_pending_write(rs1_index) ||
                                     register_file.is_register_pending_write(rs2_index);

    instr_of_B_type->set_pending_status_of_src_reg(pending_status_of_src_reg);

    if (!pending_status_of_src_reg) { // rs1 and rs2 are not pending write
        instr_of_B_type->set_rs1_value(register_file.read_a_word_by_reg_index(rs1_index));
        instr_of_B_type->set_rs2_value(register_file.read_a_word_by_reg_index(rs2_index));
    }

    return pending_status_of_src_reg;
}


tuple<JAL*, bool> InstrDecode::decode_JAL_type(unsigned int instr_word, unsigned int instr_addr) {
    unsigned int rd_index = (instr_word >> num_of_bits_to_indicate_opcode) & mask_for_register_index;
    // least significant bit of immediate (imm[0]) is always 0 and thus left out in the imm fields of an branch instruction
    // initial plan of immediate representation imm20to1
    // int imm20to1 = (static_cast<int>(instr_word) >> (num_of_bits_to_indicate_opcode
    //                                                 + num_of_bits_to_indicate_register_index)) << 1;
    // int immediate = imm20to1;

    // precisely following the RISC-V spec
    // imm[20] = word[31]
    int imm20 = (static_cast<int>(instr_word) & (1 << (num_of_bits_to_indicate_a_word - 1))) >> (31 - 20);
    // imm[10:1] = word[30:21]
    unsigned int imm10to1 = (instr_word & mask_for_J_type_imm_10_to_1) >> (21 - 1);
    // imm[11] = word[20]
    unsigned int imm11 = (instr_word & mask_for_J_type_imm_11) >> (20 - 11);
    // imm[19:12] = word[19:12]
    unsigned int imm19to12 = (instr_word & mask_for_J_type_imm_19_to_12);
    int immediate = imm20 | imm19to12 | imm11 | imm10to1;

    Instr_Name& identifier = map_for_JAL_type[0x00000000];
    // rd is the link register in JAL instruction,
    bool pending_status_of_rd = register_file.is_register_pending_write(rd_index);
    auto* a_instr_obj = new JAL(identifier, Instr_Type::JAL, rd_index, immediate, instr_addr, pending_status_of_rd);
    if (!pending_status_of_rd) { // rd is not pending write
        a_instr_obj->set_rd_value(instr_addr + 4);
        // update the link register(rd) early at decode stage
        register_file.write_a_word_by_reg_index(rd_index, instr_addr + 4);
    }

    return {a_instr_obj, pending_status_of_rd};
}

bool InstrDecode::redecode_JAL_type(Instruction* instr_obj) {
    auto* instr_of_J_type = static_cast<JAL*>(instr_obj);
    unsigned int rd_index = instr_of_J_type->get_rd_index();
    // rd is the link register in JAL instruction,
    bool pending_status_of_rd = register_file.is_register_pending_write(rd_index);
    instr_of_J_type->set_pending_status_of_rd(pending_status_of_rd);

    if (!pending_status_of_rd) { // rd is not pending write
        unsigned int return_addr = instr_of_J_type->get_pc_addr() + 4;
        instr_of_J_type->set_rd_value(return_addr);
        // update the link register(rd) early at decode stage
        register_file.write_a_word_by_reg_index(rd_index, return_addr);
    }

    return pending_status_of_rd;
}

tuple<I_JALR*, bool> InstrDecode::decode_I_JALR_type(unsigned int instr_word, unsigned int instr_addr) {
    unsigned int rd_index = ( instr_word >> num_of_bits_to_indicate_opcode) & mask_for_register_index;
    unsigned int rs1_index = ( instr_word >> (num_of_bits_to_indicate_opcode
                                             + num_of_bits_to_indicate_register_index
                                             + num_of_bits_to_indicate_funct3)) & mask_for_register_index;

    // immdiates are signed extended
    int immediate = (static_cast<int>(instr_word)) >> (num_of_bits_to_indicate_opcode
                                                       + num_of_bits_to_indicate_register_index
                                                       + num_of_bits_to_indicate_funct3
                                                       + num_of_bits_to_indicate_register_index);

    Instr_Name identifier = (instr_word & mask_for_funct3) ? Instr_Name::NO_OP : map_for_I_JALR_type[0x00000000];
    bool pending_status_of_rs1_rd = register_file.is_register_pending_write(rs1_index) ||
                                    register_file.is_register_pending_write(rd_index) ;
    auto* a_instr_obj = new I_JALR(identifier, Instr_Type::I_JALR, rd_index, rs1_index, immediate, instr_addr,
                                   pending_status_of_rs1_rd);
    if (!pending_status_of_rs1_rd) { // neither rs1 nor rd is pending write
        a_instr_obj->set_rs1_value(register_file.read_a_word_by_reg_index(rs1_index));
        a_instr_obj->set_rd_value(instr_addr + 4);
        // update the link register(rd) early at decode stage
        register_file.write_a_word_by_reg_index(rd_index, instr_addr + 4);
    }

    return {a_instr_obj, pending_status_of_rs1_rd};
}

bool InstrDecode::redecode_I_JALR_type(Instruction* instr_obj) {
    auto* instr_of_I_JALR_type = static_cast<I_JALR*>(instr_obj);
    unsigned int rs1_index = instr_of_I_JALR_type->get_rs1_index();
    unsigned int rd_index = instr_of_I_JALR_type->get_rd_index();

    bool pending_status_of_rs1_rd = register_file.is_register_pending_write(rs1_index) ||
                                    register_file.is_register_pending_write(rd_index);

    instr_of_I_JALR_type->set_pending_status_of_rs1_rd(pending_status_of_rs1_rd);

    if (!pending_status_of_rs1_rd) { // neither rs1 nor rd is pending write
        instr_of_I_JALR_type->set_rs1_value(register_file.read_a_word_by_reg_index(rs1_index));
        unsigned int return_addr = instr_of_I_JALR_type->get_pc_addr() + 4;
        instr_of_I_JALR_type->set_rd_value(return_addr);
        // update the link register(rd) early at decode stage
        register_file.write_a_word_by_reg_index(rd_index, return_addr);
    }

    return pending_status_of_rs1_rd;
}


tuple<U_AUIPC*, bool> InstrDecode::decode_U_AUIPC_type(unsigned int instr_word, unsigned int instr_addr){
    unsigned int rd_index = ( instr_word >> num_of_bits_to_indicate_opcode) & mask_for_register_index;
    // immdiates are signed extended
    /* immediate is in the upper 20 bits of a instruction word */
    int imm31to12 = static_cast<int>(instr_word & mask_for_upper_20_bits);
    int immediate = imm31to12;
    Instr_Name& identifier = map_for_U_AUIPC_type[0x00000000];
    auto* a_instr_obj = new U_AUIPC(identifier, Instr_Type::U_AUIPC, rd_index, immediate, instr_addr);

    return {a_instr_obj, false};
}


tuple<U_LUI*, bool> InstrDecode::decode_U_LUI_type(unsigned int instr_word, unsigned int instr_addr){
    unsigned int rd_index = ( instr_word >> num_of_bits_to_indicate_opcode) & mask_for_register_index;
    // immdiates are signed extended
    /* immediate is in the upper 20 bits of a instruction word */
    int imm31to12 = static_cast<int>(instr_word & mask_for_upper_20_bits);
    int immediate = imm31to12;
    Instr_Name& identifier = map_for_U_LUI_type[0x00000000];
    auto* a_instr_obj = new U_LUI(identifier, Instr_Type::U_LUI, rd_index, immediate);
    // cout << "In decode_U_LUI_type rd_value: " << a_instr_obj->get_rd_value() << endl;
    return {a_instr_obj, false};
}

tuple<HALT*, bool> InstrDecode::decode_HALT_type(unsigned int instr_word, unsigned int instr_addr){
    auto* a_instr_obj = new HALT(instr_addr);
    return {a_instr_obj, false};
}


Instruction* InstrDecode::proceed(string signal){
    // branch instruction in EX stage might need to flush the IF and ID stages
    if(signal == "flush"){
        if(on_hold_instr){
            delete on_hold_instr;
            on_hold_instr = nullptr;
        }
        // saved_instr = {false, 0, 0};
        is_last_cycle_instr_finished = true;
        is_last_cycle_instr_passed_over = true;
        saved_instr = instr_fetch.proceed(signal);
        return new STALL();
    }

    //signal not "flush"
    bool pending_status;
    Instruction* an_instr_obj;
    if(!is_last_cycle_instr_finished){ // last cycle instr is not finished
        Instr_Type instr_type = on_hold_instr->get_instr_type();
        // call the corresponding redecode function
        pending_status = map_of_various_redecode_funcs[instr_type](on_hold_instr);
        an_instr_obj = on_hold_instr;

    }else if(!is_last_cycle_instr_passed_over){ // last cycle instr is finished but not passed over
        an_instr_obj = on_hold_instr;
        pending_status = false;

    }else{ // last cycle instr is finished and passed over
        auto& [is_valid_instr, instr_word, instr_addr] = saved_instr;

        if(!is_valid_instr){ // invalid = bubble

            saved_instr = instr_fetch.proceed("permitted");
            return new STALL();
        }
        // valid instruction
        // decode a new instruction
        unsigned int opcode = instr_word & mask_for_opcode;
        auto iter = map_of_opcode_to_instr_type.find(opcode);
        if(iter == map_of_opcode_to_instr_type.end()){ // opcode not exist in ISA
            an_instr_obj = new NO_OP();
            pending_status = false;
        } else {
            Instr_Type& instr_type = iter->second;
            // call the corresponding decode function
            tie(an_instr_obj, pending_status) = map_of_various_decode_funcs[instr_type](instr_word, instr_addr);
        }

    }
    // At this position, either a on_hold_instr object has just been re-decoded, or a finished instr is yet to pass over,
    // or a new instr object has just been decoded once
    if(pending_status){ // at least one src register is pending write
        if(is_last_cycle_instr_finished && is_last_cycle_instr_passed_over)
            on_hold_instr = an_instr_obj;

        is_last_cycle_instr_finished = false;
        is_last_cycle_instr_passed_over = false;
        saved_instr = instr_fetch.proceed("blocked");
        return new STALL();
    }

    // no src reg is pending write
    if(signal == "permitted"){
        Instr_Name&& identifier = an_instr_obj->get_instr_name();
        // Because for JAL and JALR instructions, the link register(rd) are updated early at decode stage,
        // rather than at write back stage, no need to place the link register(rd) into pending-write list.
        if ( identifier != Instr_Name::JAL && identifier != Instr_Name::JALR )
            register_file.set_register_as_pending_write(an_instr_obj->get_rd_index());
        on_hold_instr = nullptr;
        is_last_cycle_instr_finished = true;
        is_last_cycle_instr_passed_over = true;
        saved_instr = instr_fetch.proceed(signal);
        return an_instr_obj;
    }else if(signal == "blocked"){
        if(is_last_cycle_instr_finished && is_last_cycle_instr_passed_over)
            on_hold_instr = an_instr_obj;

        is_last_cycle_instr_finished = true;
        is_last_cycle_instr_passed_over = false;
        saved_instr = instr_fetch.proceed(signal);
        return new STALL();
    }

    // should not reach here

    throw "Error from InstrDecode::proceed(string signal): invalid signal";
};
