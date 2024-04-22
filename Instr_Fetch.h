#ifndef INSTR_FETCH_H
#define INSTR_FETCH_H

#include "program_counter.h"
#include "Cache.h"


class InstrFetch {
public:
    InstrFetch(ProgramCounter& pc, Cache& instr_cache, bool pipeline_enabled = true);

    // tuple<is_valid_instr, instr_word, instr_addr>
    tuple<bool, unsigned int, unsigned int> proceed(string signal);

    void set_pipeline_disabled();

    void set_pipeline_enabled();

    void turn_off_switch();

    void turn_on_switch();

    void reset();

    void reset_cache(Cache& instr_cache);

private:
    ProgramCounter& program_counter;
    Cache& instr_cache;

    bool is_curr_instr_empty;
    unsigned int curr_instr;
    bool pipeline_enabled;
    // used only for pipeline_enabled == false
    bool on_off_switch;
};

#endif // INSTR_FETCH_H
