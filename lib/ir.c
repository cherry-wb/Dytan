#include "ir.h"

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

void ppIRTemp(IRTemp tmp) {
	printf("t%"PRIi8, tmp);
}

IRConst* IRConst_U32(uint32_t u32) {
	IRConst *c = (IRConst *) malloc(sizeof(IRConst));
	c->type = Const_U32;
	c->co.U32 = u32;
	return c;
}

IRConst* IRConst_U64(uint64_t u64) {
	IRConst *c = (IRConst *) malloc(sizeof(IRConst));
	c->type = Const_U64;
	c->co.U64 = u64;
	return c;
}

void ppIRConst(IRConst *cnst) {
	switch(cnst->type) {
		case Const_U32:
			printf("%#"PRIx32, cnst->co.U32);
			break;
		case Const_U64:
			printf("%#"PRIx64, cnst->co.U64);
			break;
		default:
			break;
	}
}

IRReg *IRReg_Reg(int reg) {
	IRReg *r = (IRReg *) malloc(sizeof(IRReg));
	r->reg = reg;
	r->range.start = 0;
	r->range.len = 32;
	return r;
}

void ppIRReg(IRReg *reg) {
	printf("(%d, %d, %d)", reg->reg, reg->range.start, reg->range.len);
}

IRExpr* IRExpr_Get(IRReg *reg) {
	IRExpr *e = (IRExpr *) malloc(sizeof(IRExpr));
	e->type = Expr_Get;
	e->ex.Get.reg = reg;
	return e;
}

IRExpr* IRExpr_Load(uint64_t addr) {
	IRExpr *e = (IRExpr *) malloc(sizeof(IRExpr));
	e->type = Expr_Load;
	e->ex.Load.addr = addr;
	return e;
}

IRExpr* IRExpr_Const(IRConst *cnst) {
	IRExpr *e = (IRExpr *) malloc(sizeof(IRExpr));
	e->type = Expr_Const;
	e->ex.Const.cnst = cnst;
	return e;
}

IRExpr* IRExpr_RdTmp(IRTemp tmp) {
	IRExpr *e = (IRExpr *) malloc(sizeof(IRExpr));
	e->type = Expr_RdTmp;
	e->ex.RdTmp.tmp = tmp;
	return e;
}

void ppIRExpr(IRExpr *e) {
	switch(e->type) {
		case Expr_Get:
			printf("GET");
			ppIRReg(e->ex.Get.reg);
			break;
		case Expr_Load:
			printf("LOAD(%#"PRIx64")", e->ex.Load.addr);
			break;
		case Expr_Const:
			ppIRConst(e->ex.Const.cnst);
			break;
		case Expr_RdTmp:
			ppIRTemp(e->ex.RdTmp.tmp);
			break;
		default:
			break;
	}
}


IRStmt* IRStmt_IMark(uint64_t addr, size_t len) {
	IRStmt *s = (IRStmt *) malloc(sizeof(IRStmt));
	s->type = Stmt_IMark;
	s->st.IMark.addr = addr;
	s->st.IMark.len = len;
	return s;
}

IRStmt* IRStmt_Put(IRReg *reg, IRExpr *data) {
	IRStmt *s = (IRStmt *) malloc(sizeof(IRStmt));
	s->type = Stmt_Put;
	s->st.Put.reg = reg;
	s->st.Put.data = data;
	return s;
}

IRStmt* IRStmt_Store(uint64_t addr, IRExpr *data) {
	IRStmt *s = (IRStmt *) malloc(sizeof(IRStmt));
	s->type = Stmt_Store;
	s->st.Store.addr = addr;
	s->st.Store.data = data;
	return s;
}

IRStmt* IRStmt_WrTmp(IRTemp tmp, IRExpr *data) {
	IRStmt *s = (IRStmt *) malloc(sizeof(IRStmt));
	s->type = Stmt_WrTmp;
	s->st.WrTmp.tmp = tmp;
	s->st.WrTmp.data = data;
	return s;
}

void ppIRStmt(IRStmt *stmt) {
	switch(stmt->type) {
		case Stmt_IMark:
			printf( "------ IMark(%#"PRIx64", %"PRIi32") ------", stmt->st.IMark.addr, stmt->st.IMark.len);
			break;	
		case Stmt_Put:
			printf("PUT(");
			ppIRReg(stmt->st.Put.reg);
			printf(") <- ");
			ppIRExpr(stmt->st.Put.data);
			break;	
		case Stmt_Store:
			printf("STORE(%#"PRIx64") = ", stmt->st.Store.addr);
			ppIRExpr(stmt->st.Store.data);
			break;	
		case Stmt_WrTmp:
			ppIRTemp(stmt->st.WrTmp.tmp);
			printf(" <- ");
			ppIRExpr(stmt->st.WrTmp.data);
			break;	
		default:
			break;
	}

}





IRBlock *IRBlock_New() {
	IRBlock *b = (IRBlock *) malloc(sizeof(IRBlock));
	b->stmts_used = 0;
	b->stmts_size = 8;
	b->stmts = (IRStmt **) malloc(b->stmts_size * sizeof(IRStmt *));
	return b;
}

void IRBlock_Add(IRBlock *b, IRStmt *stmt) {
	if(b->stmts_used == b->stmts_size) {
		b->stmts_size *= 2;
		b->stmts = (IRStmt **) realloc(b->stmts, b->stmts_size * sizeof(IRStmt *));
	}
	b->stmts[b->stmts_used] = stmt;
	b->stmts_used++;
}

void ppIRBlock(IRBlock *block) {
	for(int32_t i = 0; i < block->stmts_used; i++) {
      		ppIRStmt(block->stmts[i]);
		printf("\n");
	}
}
