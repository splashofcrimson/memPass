#include "llvm/IR/Constants.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/IRBuilder.h"

using namespace llvm;

namespace {
  struct MemPass : public ModulePass {
    static char ID;
    MemPass() : ModulePass(ID) {}

    virtual bool runOnModule(Module &M) {
      
      IRBuilder<>* builder = new IRBuilder<>(M.getContext());

      std::vector<Type*> argTypes;

      argTypes.push_back(Type::getInt8PtrTy(M.getContext()));
      FunctionType* logFreeType = FunctionType::get(Type::getVoidTy(M.getContext()), argTypes, false);
      Constant* logFree = M.getOrInsertFunction("logFree", logFreeType);

      std::vector<Type*> newArgs;
      newArgs.push_back(Type::getInt32PtrTy(M.getContext()));
      FunctionType* logAllocaType = FunctionType::get(Type::getVoidTy(M.getContext()), newArgs, false);
      Constant* logAlloca = M.getOrInsertFunction("logAlloca", logAllocaType);
      newArgs.push_back(Type::getInt64Ty(M.getContext()));
      FunctionType* logQueryType = FunctionType::get(Type::getVoidTy(M.getContext()), newArgs, false);
      Constant* logQuery = M.getOrInsertFunction("logQuery", logQueryType);

      argTypes.push_back(Type::getInt64Ty(M.getContext()));
      FunctionType* logMallocType = FunctionType::get(Type::getVoidTy(M.getContext()), argTypes, false);
      Constant* logMalloc = M.getOrInsertFunction("logMalloc", logMallocType);


      for(auto &F : M) {
        if (F.getName() == "logMalloc" || F.getName() == "logFree" || F.getName() == "logQuery" || F.getName() == "logAlloca") {
          continue;
        }
        for(auto &B : F) {
          for(auto &I : B) {
            if (AllocaInst* alloca = dyn_cast<AllocaInst>(&I)) {
              Value* address = cast<Value>(alloca);
              BitCastInst* bitcast = new BitCastInst(address, Type::getInt32PtrTy(M.getContext()), "a", (&I)->getNextNode());
              std::vector<Value*> args;
              Value* castAddress = cast<Value>(bitcast);
              args.push_back(castAddress);
              builder->SetInsertPoint((&I)->getNextNode()->getNextNode());

              CallInst* call = builder->CreateCall(logAlloca, args, "");
            }

            if (LoadInst* load = dyn_cast<LoadInst>(&I)) {
              if (load->getType()->getTypeID() != 0) {
                DataLayout* dataLayout = new DataLayout(&M);
                Value* address = load->getPointerOperand();
                PointerType* pointerType = cast<PointerType>(address->getType());
                uint64_t storeSize = dataLayout->getTypeStoreSize(pointerType->getPointerElementType());
                BitCastInst* bitcast = new BitCastInst(address, Type::getInt32PtrTy(M.getContext()), "l", (&I)->getNextNode());
                std::vector<Value*> args;
                Value* castAddress = cast<Value>(bitcast);
                Value* storeSizeCast = ConstantInt::get(Type::getInt64Ty(M.getContext()), storeSize);
                args.push_back(castAddress);
                args.push_back(storeSizeCast);
                builder->SetInsertPoint((&I)->getNextNode()->getNextNode());

                CallInst* call = builder->CreateCall(logQuery, args, "");
              }
            }
            
            if (StoreInst* store = dyn_cast<StoreInst>(&I)) {
              // errs() << *store->getPointerOperandType()->getTypeID() << "\n";
              if(store->getType()->getTypeID() == 11) {
                Value* address = store->getPointerOperand();
                BitCastInst* bitcast = new BitCastInst(address, Type::getInt32PtrTy(M.getContext()), "s", (&I)->getNextNode());
                std::vector<Value*> args;
                Value* castAddress = cast<Value>(bitcast);
                args.push_back(castAddress);
                builder->SetInsertPoint((&I)->getNextNode()->getNextNode());

                CallInst* call = builder->CreateCall(logQuery, args,  "");
              }
            }

            if(CallInst* call = dyn_cast<CallInst>(&I)) {
              if (call->getCalledFunction()->getName() == "malloc") {
                Value* address = cast<Value>(call);
                Value* size = call->getOperand(0);
                builder->SetInsertPoint((&I)->getNextNode());

                std::vector<Value *> args;
                args.push_back(address);
                args.push_back(size);

                CallInst* call = builder->CreateCall(logMalloc, args, "");
              }

              if (call->getCalledFunction()->getName() == "free") {
                Value* address = call->getOperand(0);
                builder->SetInsertPoint((&I)->getNextNode());

                std::vector<Type*> argTypes;

                argTypes.push_back(Type::getInt8PtrTy(M.getContext()));
                FunctionType* logNewFreeType = FunctionType::get(Type::getVoidTy(M.getContext()), argTypes, false);
                Constant* logNewFree = M.getOrInsertFunction("logFree", logNewFreeType);


                std::vector<Value *> args;
                args.push_back(address);

                CallInst* call = builder->CreateCall(logNewFree, args, "");
              }
            }
          }
        }
      }
      return true;
  
    }
  };

}

char MemPass::ID = 0;

static RegisterPass<MemPass> X("mempass", "Memory Pass", true, false);