#include "Execution.h"

Execution::Execution(ProgramCounter& pc, InstrDecode& instr_decode)
    :program_counter(pc), instr_decode(instr_decode), saved_instr(new STALL()), on_hold_instr(nullptr),
    is_last_cycle_instr_finished(true), is_last_cycle_instr_passed_over(true) {}


Execution::~Execution(){}

void Execution::reset(){
    if(saved_instr) {
        delete saved_instr;
        saved_instr = new STALL();
    }

    if(on_hold_instr){
        delete on_hold_instr;
        on_hold_instr = nullptr;
    }

    is_last_cycle_instr_finished = true;
    is_last_cycle_instr_passed_over = true;
    instr_decode.reset();
}

Instruction* Execution::get_next_instr(){
    if(on_hold_instr){
        return on_hold_instr;
    }
    return saved_instr;
}

Instruction* Execution::proceed(string signal){
    //signal cannot be "flush" to this EX stage, "flush" signal can only be sent out by EX to prior stages.
    bool need_to_flush = false;
    Instruction* an_instr_obj;
    if(!is_last_cycle_instr_finished || !is_last_cycle_instr_passed_over){ // last cycle instr is finished but not passed over
        an_instr_obj = on_hold_instr;

    }else { // last cycle instr is finished and passed over
        an_instr_obj = saved_instr;
        Instr_Type instr_type = an_instr_obj->get_instr_type();
        if(instr_type == Instr_Type::STALL){
            saved_instr = instr_decode.proceed("permitted");
            return an_instr_obj;
        }

        an_instr_obj->execute();
        if(instr_type == Instr_Type::B){
            B* branch_instr = static_cast<B*>(an_instr_obj);
            if(branch_instr->is_branch_taken()){
                program_counter.set_instruction_address_to(branch_instr->get_pc_addr());
                //need to flush the pipeline
                need_to_flush = true;
            }

        }else if(instr_type == Instr_Type::JAL){
            JAL* jal_instr = static_cast<JAL*>(an_instr_obj);
            program_counter.set_instruction_address_to(jal_instr->get_pc_addr());
            //need to flush the pipeline
            need_to_flush = true;
        }else if(instr_type == Instr_Type::I_JALR){
            I_JALR* jalr_instr = static_cast<I_JALR*>(an_instr_obj);
            program_counter.set_instruction_address_to(jalr_instr->get_pc_addr());
            //need to flush the pipeline
            need_to_flush = true;
        }
    }

    if(signal == "permitted"){
        on_hold_instr = nullptr;
        is_last_cycle_instr_finished = true;
        is_last_cycle_instr_passed_over = true;
        saved_instr = need_to_flush? instr_decode.proceed("flush"): instr_decode.proceed("permitted");
        return an_instr_obj;
    }else if(signal == "blocked"){
        on_hold_instr = an_instr_obj;
        is_last_cycle_instr_finished = true;
        is_last_cycle_instr_passed_over = false;
        saved_instr = need_to_flush? instr_decode.proceed("flush"): instr_decode.proceed("blocked");
        return new STALL();
    }

    // should not reach here

    throw "Error from Execution::proceed(string signal): invalid signal";

}
