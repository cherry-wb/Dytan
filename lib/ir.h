#ifndef __ITRACE_IR_H
#define __ITRACE_IR_H

#include "stdint.h"
#include <sys/user.h>

typedef uint8_t IRTemp;

extern void ppIRTemp(IRTemp);

typedef enum {
	Const_U32,
	Const_U64,
} IRConstType;

typedef struct _IRConst {
	IRConstType type;
	union {
		uint32_t U32;
		uint64_t U64;
	} co;
} IRConst;

extern IRConst* IRConst_U32(uint32_t);
extern IRConst* IRConst_U64(uint64_t);
extern void ppIRConst(IRConst *);

typedef struct _IRReg {
	uint32_t reg;
	struct {
		uint32_t start;
		uint32_t len;
	} range;
} IRReg;

extern IRReg *IRReg_Reg(int);
extern void ppIRReg(IRReg *);

typedef enum {
	Expr_Get,	// Read a value from a register
	Expr_Load,	// Read a value from memory
	Expr_Const,
	Expr_RdTmp,
} IRExprType;

typedef struct _IRExpr IRExpr;

struct _IRExpr {
	IRExprType type;
	union {
		struct { 
			IRReg *reg;
		} Get;
		struct {
			uint64_t addr;
		} Load;
		struct {
			IRConst *cnst;
		} Const;
		struct {
			IRTemp tmp;
		} RdTmp;
	} ex;
};

extern IRExpr* IRExpr_Get(IRReg *);
extern IRExpr* IRExpr_Load(uint64_t);
extern IRExpr* IRExpr_Const(IRConst *);
extern IRExpr* IRExpr_RdTmp(IRTemp);
extern void ppIRExpr(IRExpr *);

typedef enum {
	Stmt_IMark,
	Stmt_Put,
	Stmt_Store,
	Stmt_WrTmp,
} IRStmtType;

typedef struct _IRStmt {
	IRStmtType type;
	union {
		struct {
			uint64_t addr;
			uint32_t len;
		} IMark;
		struct { 
			IRReg *reg;
			IRExpr *data;
		} Put;
		struct { 
			uint64_t addr;
			IRExpr *data;
		} Store;
		struct {
			IRTemp tmp;
			IRExpr *data;		
		} WrTmp;
	} st;
} IRStmt;

extern IRStmt* IRStmt_IMark(uint64_t, size_t);
extern IRStmt* IRStmt_Put(IRReg *, IRExpr *);
extern IRStmt* IRStmt_Store(uint64_t, IRExpr *);
extern IRStmt* IRStmt_WrTmp(IRTemp, IRExpr *);
extern void ppIRStmt(IRStmt *);

typedef struct _IRBlock {
	IRStmt **stmts;
	int32_t stmts_size;
	int32_t stmts_used;
} IRBlock;

extern IRBlock *IRBlock_New();
extern void IRBlock_Add(IRBlock *, IRStmt *);
extern void ppIRBlock(IRBlock *);

#endif //__ITRACE_IR_H
