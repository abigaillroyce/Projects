#include "spimcore.h"


void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{
	switch (ALUControl) {
		case 0: *ALUresult = A + B; break;
		case 1: *ALUresult = A - B; break;
		case 2: *ALUresult = ((int)A < (int)B) ? 1 : 0; break;
		case 3: *ALUresult = (A < B) ? 1 : 0; break;
		case 4: *ALUresult = A & B; break;
		case 5: *ALUresult = A | B; break;
		case 6: *ALUresult = B << 16; break;
		case 7: *ALUresult = ~A; break;
		default: *ALUresult = 0; break;
	}

	*Zero = (*ALUresult == 0);
}


int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{
	if (PC % 4 == 0) {
		*instruction = Mem[PC>>2];
		return 0;
	} else {
		return 1;
	}
}


void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
	*op =		(instruction & 0xfc000000)>>26;
	*r1 =		(instruction & 0x03e00000)>>21;
	*r2 =		(instruction & 0x001f0000)>>16;
	*r3 =		(instruction & 0x0000f800)>>11;
	*funct =	(instruction & 0x0000002f)>>0;
	*offset =	(instruction & 0x0000ffff)>>0;
	*jsec =	(instruction & 0x03ffffff)>>0;
	return;
}


int instruction_decode(unsigned op,struct_controls *controls)
{
	switch (op) {
		
		case 0:
			controls->RegDst = 1;
			controls->Jump = 0;
			controls->Branch = 0;
			controls->MemRead = 0;
			controls->MemtoReg = 0;
			controls->ALUOp = 7;
			controls->MemWrite = 0;
			controls->ALUSrc = 0;
			controls->RegWrite = 1;
			break;
		case 2:
			controls->RegDst = 0;
			controls->Jump = 1;
			controls->Branch = 0;
			controls->MemRead = 0;
			controls->MemtoReg = 0;
			controls->ALUOp = 0;
			controls->MemWrite = 0;
			controls->ALUSrc = 0;
			controls->RegWrite = 0;
			break;
		case 4:
			controls->RegDst = 2;
			controls->Jump = 0;
			controls->Branch = 1;
			controls->MemRead = 0;
			controls->MemtoReg = 2;
			controls->ALUOp = 1;
			controls->MemWrite = 0;
			controls->ALUSrc = 0;
			controls->RegWrite = 0;
			break;
		case 8:
			controls->RegDst = 0;
			controls->Jump = 0;
			controls->Branch = 0;
			controls->MemRead = 0;
			controls->MemtoReg = 0;
			controls->ALUOp = 0;
			controls->MemWrite = 0;
			controls->ALUSrc = 1;
			controls->RegWrite = 1;
			break;
		case 15:
			controls->RegDst = 0;
			controls->Jump = 0;
			controls->Branch = 0;
			controls->MemRead = 0;
			controls->MemtoReg = 0;
			controls->ALUOp = 6;
			controls->MemWrite = 0;
			controls->ALUSrc = 1;
			controls->RegWrite = 1;
			break;
		case 35:
			controls->RegDst = 0;
			controls->Jump = 0;
			controls->Branch = 0;
			controls->MemRead = 1;
			controls->MemtoReg = 1;
			controls->ALUOp = 0;
			controls->MemWrite = 0;
			controls->ALUSrc = 1;
			controls->RegWrite = 1;
			break;
		case 43:
			controls->RegDst = 2;
			controls->Jump = 0;
			controls->Branch = 0;
			controls->MemRead = 0;
			controls->MemtoReg = 2;
			controls->ALUOp = 0;
			controls->MemWrite = 1;
			controls->ALUSrc = 1;
			controls->RegWrite = 0;
			break;
		default:
			return 1;
	}
	return 0;
}


void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{
	*data1 = Reg[r1];
	*data2 = Reg[r2];
}


void sign_extend(unsigned offset,unsigned *extended_value)
{
	if (offset >> 15 == 1) {
		*extended_value = offset | 0xffff0000;
	} else {
		*extended_value = offset & 0x0000ffff;
	}

	return;
}


int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{
	unsigned B = ALUSrc ? extended_value : data2;
	char control = ALUOp;
	if (ALUOp == 7) {
		switch (funct) {
			case 32: control = 0; break;
			case 34: control = 1; break;
			case 42: control = 2; break;
			case 43: control = 3; break;
			case 36: control = 4; break;
			case 37: control = 5; break;
			case 0:  control = 6; break;
			default: return 1;
		}
	}
	ALU(data1, B, control, ALUresult, Zero);
	return 0;
}


void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{
    *PC += 4;
     
    if (Jump == 1) *PC = (jsec<<2) | (*PC & 0xf0000000);
    if (Branch == 1 && Zero == 1) *PC += extended_value << 2;
}


int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{
	if (MemWrite == 1) {
		if (ALUresult%4 == 0) Mem[ALUresult>>2] = data2;
		else return 1;
	}

	if (MemRead == 1) {
		if (ALUresult%4 == 0) *memdata = Mem[ALUresult>>2];
		else return 1;
	}
	return 0;
}


void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{
	if (RegWrite == 1) {
		if (MemtoReg == 0) {
			if (RegDst == 0) Reg[r2] = ALUresult;
			else Reg[r3] = ALUresult;
		} else {
			if (RegDst == 0) Reg[r2] = memdata;
			else Reg[r3] = memdata;
		}
	}
}

