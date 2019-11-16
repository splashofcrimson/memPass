#include "llvm/Pass.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/DebugLoc.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/DebugInfoMetadata.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
using namespace llvm;

namespace {
  struct SkeletonPass : public ModulePass {
    static char ID;
    SkeletonPass() : ModulePass(ID) {}

    struct Metadata {
      int64_t line;
      int64_t col;
    };

    struct Metadata getLineAndCol(Instruction &I) {
      struct Metadata instrMetadata = {-1, -1};
      const DebugLoc &debugInfo = I.getDebugLoc();
      if (debugInfo) {
        instrMetadata.line = (int64_t)debugInfo.getLine();
        instrMetadata.col =  (int64_t)debugInfo.getCol();
      }
      return instrMetadata;
    }

    virtual bool runOnModule(Module &M) {

      auto &context = M.getContext();
      auto VoidTy = Type::getVoidTy(context);
      auto Int64Ty = Type::getInt64Ty(context);
      auto Int8PtrTy = Type::getInt8PtrTy(context);
      auto Int32PtrTy = Type::getInt32PtrTy(context);

      // Register logging functions from runtime library
      // BEGIN
      Constant* init = M.getOrInsertFunction("initialize",
        FunctionType::get(
          VoidTy, false
        )
      );

      std::vector<Type*> mallocArgs;
      mallocArgs.push_back(Int8PtrTy);
      mallocArgs.push_back(Int64Ty);
      mallocArgs.push_back(Int64Ty);
      mallocArgs.push_back(Int64Ty);
      Constant* log_malloc = M.getOrInsertFunction("logMalloc",
        FunctionType::get(
          VoidTy, mallocArgs, false
        )
      );

      std::vector<Type*> allocArgs;
      allocArgs.push_back(Int32PtrTy);
      Constant* log_alloca = M.getOrInsertFunction("logAlloca",
        FunctionType::get(
          VoidTy, allocArgs, false
        )
      );

      std::vector<Type*> freeArgs;
      freeArgs.push_back(Int8PtrTy);
      freeArgs.push_back(Int64Ty);
      freeArgs.push_back(Int64Ty);
      Constant* log_free = M.getOrInsertFunction("logFree",
        FunctionType::get(
          VoidTy, freeArgs, false
        )
      );

      std::vector<Type*> queryArgs;
      queryArgs.push_back(Int32PtrTy);
      queryArgs.push_back(Int64Ty);
      queryArgs.push_back(Int64Ty);
      queryArgs.push_back(Int64Ty);
      Constant* log_query = M.getOrInsertFunction("logQuery",
        FunctionType::get(
          VoidTy, queryArgs, false
        )
      );
      // END

      // Initialize data structures
      // BEGIN
      Function* main = M.getFunction("main");
      BasicBlock* first_bb = &*main->begin();
      BasicBlock::iterator insertion_pt = first_bb->getFirstInsertionPt();

      IRBuilder<>* builder = new IRBuilder<>(context);
      builder->SetInsertPoint(&*insertion_pt);
      CallInst* call = builder->CreateCall(init);
      // END

      // Iterate over instructions and insert library function calls
      // BEGIN
      for (auto &F : M) {
        for (auto &B : F) {
          for (auto &I : B) {
            // Alloca: Log i32* stack address
            if (AllocaInst* alloca = dyn_cast<AllocaInst>(&I)) {
              BitCastInst* bitcast = new BitCastInst(
                cast<Value>(alloca), Int32PtrTy, "a", (&I)->getNextNode()
              );

              Value* castAddress = cast<Value>(bitcast);
              std::vector<Value*> args;
              args.push_back(castAddress);

              builder->SetInsertPoint((&I)->getNextNode()->getNextNode());
              CallInst* call = builder->CreateCall(log_alloca, args, "");
            }

            // Malloc/Calloc: Log i8* address and i64 size
            if (CallInst* call = dyn_cast<CallInst>(&I)) {
              if (call->getCalledFunction()->getName() == "malloc") {
                Value* address = cast<Value>(call);
                Value* size = call->getOperand(0);
                struct Metadata instrMetadata = getLineAndCol(I);

                std::vector<Value*> args;
                args.push_back(address);
                args.push_back(size);
                args.push_back(ConstantInt::get(
                  Int64Ty, instrMetadata.line, true
                ));
                args.push_back(ConstantInt::get(
                  Int64Ty, instrMetadata.col, true
                ));

                builder->SetInsertPoint((&I)->getNextNode());
                CallInst* call = builder->CreateCall(log_malloc, args, "");
              }

              if(call->getCalledFunction()->getName() == "calloc") {
                ConstantInt* amount = dyn_cast<ConstantInt>(call->getOperand(0));
                ConstantInt* sizeOfOne = dyn_cast<ConstantInt>(call->getOperand(1));
                Value* size = ConstantInt::get(
                  Int64Ty,
                  amount->getSExtValue() * sizeOfOne->getSExtValue()
                );
                Value* address = cast<Value>(call);
                struct Metadata instrMetadata = getLineAndCol(I);

                std::vector<Value*> args;
                args.push_back(address);
                args.push_back(size);
                args.push_back(ConstantInt::get(
                  Int64Ty, instrMetadata.line, true
                ));
                args.push_back(ConstantInt::get(
                  Int64Ty, instrMetadata.col, true
                ));

                builder->SetInsertPoint((&I)->getNextNode());
                CallInst* call = builder->CreateCall(log_malloc, args, "");
              }

              //Free: Log i8* address, i64 line#, and i64 col#
              if (call->getCalledFunction()->getName() == "free") {
                Value* address = call->getOperand(0);
                struct Metadata instrMetadata = getLineAndCol(I);

                std::vector<Value*> args;
                args.push_back(address);
                args.push_back(ConstantInt::get(
                  Int64Ty, instrMetadata.line, true
                ));
                args.push_back(ConstantInt::get(
                  Int64Ty, instrMetadata.col, true
                ));

                builder->SetInsertPoint((&I)->getNextNode());
                CallInst* call = builder->CreateCall(log_free, args, "");
              }
            }

            //Query: Log i8* address, i64 size, i64 line#, and i64 col#
            if (LoadInst* load = dyn_cast<LoadInst>(&I)) {
              Value* uncastAddress = load->getPointerOperand();
              BitCastInst* bitcast = new BitCastInst(
                uncastAddress, Int32PtrTy, "l", (&I)->getNextNode()
              );
              DataLayout* datalayout = new DataLayout(&M);
              PointerType* pointerType = cast<PointerType>(uncastAddress->getType());
              uint64_t unsignSize = datalayout->getTypeStoreSize(pointerType->getPointerElementType());
              struct Metadata instrMetadata = getLineAndCol(I);
              
              Value* address = cast<Value>(bitcast);
              Value* size = ConstantInt::get(Int64Ty, unsignSize);

              std::vector<Value*> args;
              args.push_back(address);
              args.push_back(size);
              args.push_back(ConstantInt::get(
                Int64Ty, instrMetadata.line, true
              ));
              args.push_back(ConstantInt::get(
                Int64Ty, instrMetadata.col, true
              ));

              builder->SetInsertPoint((&I)->getNextNode()->getNextNode());
              CallInst* call = builder->CreateCall(log_query, args, "");
            }

            if (StoreInst* store = dyn_cast<StoreInst>(&I)) {
              Value* uncastAddress = store->getPointerOperand();
              BitCastInst* bitcast = new BitCastInst(
                uncastAddress, Int32PtrTy, "l", (&I)->getNextNode()
              );
              DataLayout* datalayout = new DataLayout(&M);
              PointerType* pointerType = cast<PointerType>(uncastAddress->getType());
              uint64_t unsignSize = datalayout->getTypeStoreSize(pointerType->getPointerElementType());
              struct Metadata instrMetadata = getLineAndCol(I);

              Value* address = cast<Value>(bitcast);
              Value* size = ConstantInt::get(Int64Ty, unsignSize);

              std::vector<Value*> args;
              args.push_back(address);
              args.push_back(size);
              args.push_back(ConstantInt::get(
                Int64Ty, instrMetadata.line, true
              ));
              args.push_back(ConstantInt::get(
                Int64Ty, instrMetadata.col, true
              ));

              builder->SetInsertPoint((&I)->getNextNode()->getNextNode());
              CallInst* call = builder->CreateCall(log_query, args, "");
            }
          }
        }
      }
      return true;
    }
  };
}

char SkeletonPass::ID = 0;

// Register the pass so `opt -mempass` runs it.
static RegisterPass<SkeletonPass> X("mempass", "detecting memory vulnerabilities");