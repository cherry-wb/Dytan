#include "decode.h"

#include <stdlib.h>

#include "ir.h"

IRBlock *decode(ud_t *udis) {

	IRBlock *block = IRBlock_New();
	
	IRStmt *imark = IRStmt_IMark(ud_insn_off(udis), ud_insn_len(udis));
	IRBlock_Add(block, imark);

	switch (udis->mnemonic) {
		case UD_Imov: {
			// source operand
			ud_operand_t *src = &(udis->operand[1]);
			IRTemp srcTmp = 0xff;
			switch(src->type) {
				case UD_OP_MEM:
					break;
				case UD_OP_REG: {
					IRExpr *e = IRExpr_Get(IRReg_Reg(src->base));
					IRStmt *s = IRStmt_WrTmp(0, e);
					IRBlock_Add(block, s);
					break;
				}
				default:
					break;			
			}
			// destination operand
			ud_operand_t *dest = &(udis->operand[0]);
			switch(dest->type) {
				case UD_OP_MEM:
					break;
				case UD_OP_REG: {
					IRExpr *e = IRExpr_RdTmp(srcTmp);
					IRStmt *s = IRStmt_Put(IRReg_Reg(dest->base), e);
					IRBlock_Add(block, s);
					break;
				}
				default:
					break;			
			}
			break;
		}
		default:
			fprintf(stderr, "[%#"PRIx64"] %-24s unsupported\n", ud_insn_off(udis), ud_insn_asm(udis));
			exit(1);
			break;	
	}
	
	return block;
}
