#include "itrace.h"

#include <stdlib.h>
#include <assert.h>
#include <inttypes.h>

#include "ir.h"
#include "udis86.h"
#include "decode.h"

struct _itrace_t {
	ud_t *udis;	
	peek_mem_func_t peek_mem;
	peek_reg_func_t peek_reg;
};

itrace_t *itrace_init() {
	itrace_t *tracer;

	tracer = (itrace_t *) malloc(sizeof(*tracer));
	assert(tracer);

	tracer->udis = (ud_t *) malloc(sizeof(*(tracer->udis)));

	ud_init(tracer->udis);
	ud_set_mode(tracer->udis, 32);
	ud_set_syntax(tracer->udis, UD_SYN_INTEL);

        return tracer;
}

void itrace_free(itrace_t *tracer) {
	free(tracer->udis);
	free(tracer);
}

void itrace_set_peek_reg_func(itrace_t *tracer, peek_reg_func_t f) {
	tracer->peek_reg = f;
}

void itrace_set_peek_mem_func(itrace_t *tracer, peek_mem_func_t f) {
	tracer->peek_mem = f;
}

void itrace_trace(itrace_t *tracer, uint32_t eip) {
	uint8_t code[12];
	tracer->peek_mem(code, sizeof(code), eip);

	ud_set_pc(tracer->udis, eip);
	ud_set_input_buffer(tracer->udis, code, sizeof(code));
	ud_disassemble(tracer->udis);

	printf("[%#"PRIx32"] %-24s\n", eip, ud_insn_asm(tracer->udis));

	IRBlock *block = decode(tracer->udis);
	ppIRBlock(block);	
}
