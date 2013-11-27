#ifndef __ITRACE_H__
#define __ITRACE_H__

#include <stdint.h>
#include <sys/user.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct _itrace_t itrace_t;

typedef uint32_t reg_t;

typedef void (*peek_mem_func_t)(uint8_t *buf, size_t cnt, off_t addr);
typedef uint32_t (*peek_reg_func_t)(reg_t reg);

extern itrace_t *itrace_init();
extern void itrace_free(itrace_t *tracer);

extern void itrace_set_peek_mem_func(itrace_t *tracer, peek_mem_func_t f);
extern void itrace_set_peek_reg_func(itrace_t *tracer, peek_reg_func_t f);

extern void itrace_trace(itrace_t *tracer, uint32_t eip);

#endif
