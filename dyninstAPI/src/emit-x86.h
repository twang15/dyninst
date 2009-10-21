/*
 * Copyright (c) 1996-2004 Barton P. Miller
 * 
 * We provide the Paradyn Parallel Performance Tools (below
 * described as "Paradyn") on an AS IS basis, and do not warrant its
 * validity or performance.  We reserve the right to update, modify,
 * or discontinue this software at any time.  We shall have no
 * obligation to supply such updates or modifications or any other
 * form of support to you.
 * 
 * This license is for research uses.  For such uses, there is no
 * charge. We define "research use" to mean you may freely use it
 * inside your organization for whatever purposes you see fit. But you
 * may not re-distribute Paradyn or parts of Paradyn, in any form
 * source or binary (including derivatives), electronic or otherwise,
 * to any other organization or entity without our permission.
 * 
 * (for other uses, please contact us at paradyn@cs.wisc.edu)
 * 
 * All warranties, including without limitation, any warranty of
 * merchantability or fitness for a particular purpose, are hereby
 * excluded.
 * 
 * By your use of Paradyn, you understand and agree that we (or any
 * other person or entity with proprietary rights in Paradyn) are
 * under no obligation to provide either maintenance services,
 * update services, notices of latent defects, or correction of
 * defects for Paradyn.
 * 
 * Even if advised of the possibility of such damages, under no
 * circumstances shall we (or any other person or entity with
 * proprietary rights in the software licensed hereunder) be liable
 * to you or any third party for direct, indirect, or consequential
 * damages of any character regardless of type of action, including,
 * without limitation, loss of profits, loss of use, loss of good
 * will, or computer failure or malfunction.  You agree to indemnify
 * us (and any other person or entity with proprietary rights in the
 * software licensed hereunder) for any and all liability it may
 * incur to third parties resulting from your use of Paradyn.
 */

/*
 * emit-x86.h - x86 & AMD64 code generators
 * $Id: emit-x86.h,v 1.32 2008/09/11 20:14:14 mlam Exp $
 */

#ifndef _EMIT_X86_H
#define _EMIT_X86_H

#include "common/h/headers.h"
#include "dyninstAPI/src/instPoint.h"
#include "dyninstAPI/src/arch-x86.h"
#include "dyninstAPI/src/baseTramp.h"

#include "dyninstAPI/src/emitter.h"
class codeGen;
class registerSpace;

class registerSlot;

// Emitter moved to emitter.h - useful on other platforms as well

// 32-bit class declared here since its implementation is in both inst-x86.C and emit-x86.C
class EmitterIA32 : public Emitter {

public:
    virtual ~EmitterIA32() {};
    static const int mt_offset;
    codeBufIndex_t emitIf(Register expr_reg, Register target, RegControl rc, codeGen &gen);
    void emitOp(unsigned opcode, Register dest, Register src1, Register src2, codeGen &gen);
    void emitRelOp(unsigned op, Register dest, Register src1, Register src2, codeGen &gen);
    void emitDiv(Register dest, Register src1, Register src2, codeGen &gen);
    void emitOpImm(unsigned opcode1, unsigned opcode2, Register dest, Register src1, RegValue src2imm,
			   codeGen &gen);
    void emitRelOpImm(unsigned op, Register dest, Register src1, RegValue src2imm, codeGen &gen);
    void emitTimesImm(Register dest, Register src1, RegValue src1imm, codeGen &gen);
    void emitDivImm(Register dest, Register src1, RegValue src1imm, codeGen &gen);
    void emitLoad(Register dest, Address addr, int size, codeGen &gen);
    void emitLoadConst(Register dest, Address imm, codeGen &gen);
    void emitLoadIndir(Register dest, Register addr_reg, int size, codeGen &gen);
    bool emitLoadRelative(Register dest, Address offset, Register base, codeGen &gen);
    bool emitLoadRelative(registerSlot* /*dest*/, Address /*offset*/, registerSlot* /*base*/, codeGen &/*gen*/) { assert(0); return true; }
    void emitLoadShared(opCode op, Register dest, const image_variable *var, bool is_local,int size, codeGen &gen, Address offset);
    void emitLoadFrameAddr(Register dest, Address offset, codeGen &gen);
    void emitLoadOrigFrameRelative(Register dest, Address offset, codeGen &gen);
    void emitLoadOrigRegRelative(Register dest, Address offset, Register base, codeGen &gen, bool store);
    void emitLoadOrigRegister(Address register_num, Register dest, codeGen &gen);

    void emitStoreOrigRegister(Address register_num, Register dest, codeGen &gen);

    void emitStore(Address addr, Register src, int size, codeGen &gen);
    void emitStoreIndir(Register addr_reg, Register src, int size, codeGen &gen);
    void emitStoreFrameRelative(Address offset, Register src, Register scratch, int size, codeGen &gen);

    void emitStoreRelative(Register source, Address offset, Register base, codeGen &gen);
    void emitStoreRelative(registerSlot* /*source*/, Address /*offset*/, registerSlot* /*base*/, codeGen &/*gen*/) { assert(0); }

    void emitStoreShared(Register source, const image_variable *var, bool is_local,int size, codeGen &gen);

    bool clobberAllFuncCall(registerSpace *rs,int_function *callee);
    void setFPSaveOrNot(const int * liveFPReg,bool saveOrNot);
    // We can overload this for the stat/dyn case
    virtual Register emitCall(opCode op, codeGen &gen,
                              const pdvector<AstNodePtr> &operands,
                              bool noCost, int_function *callee);
    int emitCallParams(codeGen &gen, 
                       const pdvector<AstNodePtr> &operands,
                       int_function *target, 
                       pdvector<Register> &extra_saves,
                       bool noCost);
    bool emitCallCleanup(codeGen &gen, int_function *target, 
                         int frame_size, pdvector<Register> &extra_saves);
    void emitGetRetVal(Register dest, bool addr_of, codeGen &gen);
    void emitGetParam(Register dest, Register param_num, instPointType_t pt_type, bool addr_of, codeGen &gen);
    void emitFuncJump(int_function *f, instPointType_t ptType, codeGen &gen);
    void emitASload(int ra, int rb, int sc, long imm, Register dest, codeGen &gen);
    void emitCSload(int ra, int rb, int sc, long imm, Register dest, codeGen &gen);
    void emitPushFlags(codeGen &gen);
    void emitRestoreFlags(codeGen &gen, unsigned offset);
    void emitRestoreFlagsFromStackSlot(codeGen &gen);
    bool emitBTSaves(baseTramp* bt, baseTrampInstance *, codeGen &gen);
    bool emitBTRestores(baseTramp* bt, baseTrampInstance *bti, codeGen &gen);
    void emitLoadEffectiveAddress(Register base, Register index, unsigned int scale, int disp,
				  Register dest, codeGen &gen);
    void emitStoreImm(Address addr, int imm, codeGen &gen, bool noCost);
    void emitAddSignedImm(Address addr, int imm, codeGen &gen, bool noCost);
    int Register_DWARFtoMachineEnc(int n);
    bool emitPush(codeGen &gen, Register pushee);
    bool emitPop(codeGen &gen, Register popee);

    bool emitAdjustStackPointer(int index, codeGen &gen);

    bool emitMoveRegToReg(Register src, Register dest, codeGen &gen);
    bool emitMoveRegToReg(registerSlot* /*src*/, registerSlot* /*dest*/, codeGen& /*gen*/) { assert(0); return true; }


 protected:
    virtual bool emitCallInstruction(codeGen &gen, int_function *target, Register ret) = 0;

};

class EmitterIA32Dyn : public EmitterIA32 {
 public:
    ~EmitterIA32Dyn() {};
    
 protected:
    bool emitCallInstruction(codeGen &gen, int_function *target, Register ret);
};

class EmitterIA32Stat : public EmitterIA32 {
 public:

    ~EmitterIA32Stat() {};
    
 protected:
    bool emitCallInstruction(codeGen &gen, int_function *target, Register ret);
};

extern EmitterIA32Dyn emitterIA32Dyn;
extern EmitterIA32Stat emitterIA32Stat;


// some useful 64-bit codegen functions
void emitMovRegToReg64(Register dest, Register src, bool is_64, codeGen &gen);
void emitMovPCRMToReg64(Register dest, int offset, int size, codeGen &gen);
void emitMovImmToReg64(Register dest, long imm, bool is_64, codeGen &gen);
void emitLEA64(Register base, Register index, unsigned int scale, int disp, Register dest, bool is_64, codeGen &gen);
void emitPushReg64(Register src, codeGen &gen);
void emitPopReg64(Register dest, codeGen &gen);
void emitMovImmToRM64(Register base, int disp, int imm, codeGen &gen);
void emitAddMem64(Address addr, int imm, codeGen &gen);
void emitAddRM64(Address addr, int imm, codeGen &gen);

#if defined(arch_x86_64)
class EmitterAMD64 : public Emitter {

public:
    virtual ~EmitterAMD64() {};
    static const int mt_offset;
    codeBufIndex_t emitIf(Register expr_reg, Register target, RegControl rc, codeGen &gen);
    void emitOp(unsigned op, Register dest, Register src1, Register src2, codeGen &gen);
    void emitRelOp(unsigned op, Register dest, Register src1, Register src2, codeGen &gen);
    void emitDiv(Register dest, Register src1, Register src2, codeGen &gen);
    void emitOpImm(unsigned opcode1, unsigned opcode2, Register dest, Register src1, RegValue src2imm,
			   codeGen &gen);
    void emitRelOpImm(unsigned op, Register dest, Register src1, RegValue src2imm, codeGen &gen);
    void emitTimesImm(Register dest, Register src1, RegValue src1imm, codeGen &gen);
    void emitDivImm(Register dest, Register src1, RegValue src1imm, codeGen &gen);
    void emitLoad(Register dest, Address addr, int size, codeGen &gen);
    void emitLoadConst(Register dest, Address imm, codeGen &gen);
    void emitLoadIndir(Register dest, Register addr_reg, int size, codeGen &gen);
    bool emitLoadRelative(Register dest, Address offset, Register base, codeGen &gen);
    bool emitLoadRelative(registerSlot *dest, Address offset, registerSlot *base, codeGen &gen);
    void emitLoadFrameAddr(Register dest, Address offset, codeGen &gen);

    void emitLoadOrigFrameRelative(Register dest, Address offset, codeGen &gen);
    void emitLoadOrigRegRelative(Register dest, Address offset, Register base, codeGen &gen, bool store);
    void emitLoadOrigRegister(Address register_num, Register dest, codeGen &gen);
    void emitLoadShared(opCode op, Register dest, const image_variable *var, bool is_local,int size, codeGen &gen, Address offset);

    void emitStoreOrigRegister(Address register_num, Register dest, codeGen &gen);

    void emitStore(Address addr, Register src, int size, codeGen &gen);
    void emitStoreIndir(Register addr_reg, Register src, int size, codeGen &gen);
    void emitStoreFrameRelative(Address offset, Register src, Register scratch, int size, codeGen &gen);
    void emitStoreRelative(Register source, Address offset, Register base, codeGen &gen);

    void emitStoreRelative(registerSlot *source, Address offset, registerSlot *base, codeGen &gen);
    void emitStoreShared(Register source, const image_variable *var, bool is_local,int size, codeGen &gen);

    bool clobberAllFuncCall(registerSpace *rs, int_function *callee);
    void setFPSaveOrNot(const int * liveFPReg,bool saveOrNot);
    // See comment on 32-bit emitCall
    virtual Register emitCall(opCode op, codeGen &gen,
                              const pdvector<AstNodePtr> &operands,
                              bool noCost, int_function *callee);
    void emitGetRetVal(Register dest, bool addr_of, codeGen &gen);
    void emitGetParam(Register dest, Register param_num, instPointType_t pt_type, bool addr_of, codeGen &gen);
    void emitFuncJump(int_function *f, instPointType_t ptType, codeGen &gen);
    void emitASload(int ra, int rb, int sc, long imm, Register dest, codeGen &gen);
    void emitCSload(int ra, int rb, int sc, long imm, Register dest, codeGen &gen);
    void emitPushFlags(codeGen &gen);
    void emitRestoreFlags(codeGen &gen, unsigned offset);
    void emitRestoreFlagsFromStackSlot(codeGen &gen);
    bool emitBTSaves(baseTramp* bt, baseTrampInstance *, codeGen &gen);
    bool emitBTRestores(baseTramp* bt, baseTrampInstance *bti, codeGen &gen);
    void emitStoreImm(Address addr, int imm, codeGen &gen, bool noCost);
    void emitAddSignedImm(Address addr, int imm, codeGen &gen, bool noCost);
    /* The DWARF register numbering does not correspond to the architecture's
       register encoding for 64-bit target binaries *only*. This method
       maps the number that DWARF reports for a register to the actual
       register number. */
    int Register_DWARFtoMachineEnc(int n);
    bool emitPush(codeGen &gen, Register pushee);
    bool emitPop(codeGen &gen, Register popee);

    bool emitAdjustStackPointer(int index, codeGen &gen);

    bool emitMoveRegToReg(Register src, Register dest, codeGen &gen);
    bool emitMoveRegToReg(registerSlot *src, registerSlot *dest, codeGen &gen);

 protected:
    virtual bool emitCallInstruction(codeGen &gen, int_function *target, Register ret) = 0;

};

class EmitterAMD64Dyn : public EmitterAMD64 {
 public:
    ~EmitterAMD64Dyn() {}

    bool emitCallInstruction(codeGen &gen, int_function *target, Register ret);
};

class EmitterAMD64Stat : public EmitterAMD64 {
 public:
    ~EmitterAMD64Stat() {};
    
    bool emitCallInstruction(codeGen &gen, int_function *target, Register ret);
};

extern EmitterAMD64Dyn emitterAMD64Dyn;
extern EmitterAMD64Stat emitterAMD64Stat;

/* useful functions for inter-library function/variable references
 * (used in the binary rewriter) */
Address getInterModuleFuncAddr(int_function *func, codeGen& gen);
Address getInterModuleVarAddr(const image_variable *var, codeGen& gen);

#endif

#endif
