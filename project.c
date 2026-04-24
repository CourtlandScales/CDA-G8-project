/*
CDA 3103C: Computer Logic and Organization
Dr. John Aedo
Spring 2026
Authors: Courtland Scales, Marco Morris
Programming language: C
Date: 4/8/2026 
*/

#include "spimcore.h"
/* ALU */
/* 10 Points */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{
    switch(ALUControl){
        case 0: // add 000
            *ALUresult = A + B;  
            break;
        case 1: // sub 001
            *ALUresult = A - B;  
            break;
        case 2: // less than 010
            if ((int)A < (int)B){  
                *ALUresult = 1;
            }    
            else{
                *ALUresult = 0;
            }
            break;
        case 3: // less than unsign 011
            if (A < B){     
               *ALUresult = 1; 
            }
            else{
                *ALUresult = 0;
            }
            break;   
        case 4: // and 100
            *ALUresult = (A & B); 
            break;
        case 5: // or 101
            *ALUresult = (A | B); 
            break;
        case 6: // shift B by 16 110
            *ALUresult = B << 16; 
            break;
        case 7: // not A 111
            *ALUresult = ~A;
            break;
    }
    
    if (*ALUresult == 0) {
        *Zero = 1;
    } else {
        *Zero = 0;
    }
    
}
/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{
    if ((PC & 3) != 0 || PC > 0xFFFF){
        return 1;
    }
    
    *instruction = Mem[PC >> 2];
    
    return 0;
}
/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned
*r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
    *op = (instruction >> 26) & 0x3F; // instruction [31-26]
    *r1 = (instruction >> 21) & 0x1F; // instruction [25-21]
    *r2 = (instruction >> 16) & 0x1F; // instruction [20-16]
    *r3 = (instruction >> 11) & 0x1F; // instruction [15-11]
    *funct = instruction & 0x3F;   // instruction [5-0]
    *offset = instruction & 0xFFFF;  // instruction [15-0]
    *jsec = instruction & 0x3FFFFFF;  // instruction [25-0]
    
}
/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op,struct_controls *controls)
{
    (*controls).RegDst = 0;
    (*controls).Jump = 0;
    (*controls).Branch = 0;
    (*controls).MemRead = 0;
    (*controls).MemtoReg = 0;
    (*controls).ALUOp = 0;
    (*controls).MemWrite = 0;
    (*controls).ALUSrc = 0;
    (*controls).RegWrite = 0;
    
    switch(op){
        case 0: //add, sub, and, or, slt, & sltu Appendix A F1
            (*controls).RegDst = 1;
            (*controls).ALUOp = 7;
            (*controls).RegWrite = 1;
            break;
        case 2: // jump Appendix A F1
            (*controls).Jump = 1;
            (*controls).MemtoReg = 2;
            (*controls).ALUSrc = 2;
            (*controls).ALUOp = 2;
            (*controls).RegWrite = 0;
            break;
        case 4: // beq Appendix A F1
            (*controls).RegDst = 2;     
            (*controls).Branch = 1;     
            (*controls).MemtoReg = 2;   
            (*controls).ALUOp = 1;      
            break;
        case 8: // addi Appendix A F1
            (*controls).ALUSrc = 1;
            (*controls).RegWrite = 1;
            break;
        case 9: // addiu Appendix A F1 
            (*controls).ALUSrc = 1;
            (*controls).RegWrite = 1;
            (*controls).ALUOp = 0;
            break;
        case 13: // ori Appendix A F1
            (*controls).RegWrite = 1;
            (*controls).ALUSrc = 1;
            (*controls).ALUOp = 5;
            break;
        case 15: // lui Appendix A F1
            (*controls).RegWrite = 1;
            (*controls).ALUSrc = 1;
            (*controls).ALUOp = 6;
            break;
        case 35: // lw Appendix A F1
            (*controls).ALUOp = 0;
            (*controls).RegWrite = 1;
            (*controls).MemRead = 1;
            (*controls).ALUSrc = 1;
            (*controls).MemtoReg = 1;
            break;
        case 43: // sw Appendix A F1
            (*controls).MemWrite = 1;
            (*controls).ALUSrc = 1;
            (*controls).RegDst = 2;
            (*controls).MemtoReg = 2;
            (*controls).ALUOp = 0;
            break;
        default:
            return 1;
        
    }
    return 0;
}
/* Read Register */
/* 5 Points */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned
*data2)
{
   *data1 = Reg[r1]; //value from memory to register
   *data2 = Reg[r2]; //value from memory to register
    
}
/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset,unsigned *extended_value)
{
    //first extending offset with leading zeros
    if(((offset >> 15) & 1)) //using AND to check if the offset is signed to be - or +
    {
        *extended_value= 0xFFFF0000 | offset; //setting the first like 16 bits to 1 
    } 
    else //if the offset is positive 
    {
        *extended_value=0xFFFF0000 & offset; //setting the new set of bits in the 32 bit to zero 
    }
}
/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned
funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{
    //since the operations later on are based on data2 and extended aswell as ALUSrc ALU will performed on them first
    ALU(data2,extended_value,ALUrc,*AlUresult,*Zero); 
    
    funct=*Zero; //saving the value for the use of determining the future operation
    
    //determining the values to be put into main ALU function using ALSUrc 
    
    switch(funct):
        case 0://for the case of ALCrc begetting 0
            ALU(data1,data2,ALUp,*AlUresult,*Zero);
            break;
        case 1://for the case of ALCrc begetting 1
            ALU(data1,extended_value,ALUp,*AlUresult,*Zero);
            break;
            
    //check if ALUresult is empty/ if there was a halt 
    if(ALUresult==NULL)
        return 1;
    else
        return 0;
    
}
/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned
*memdata,unsigned *Mem)
{
}
/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned
ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{
}
/* PC update */
/* 10 Points */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char
Zero,unsigned *PC)
{
}
