//header file
#pragma once
#include "Mutator_base.h"

/**
 * Mutate_Function_Return_Type_With_Decltype_152
 */ 
class MutatorFrontendAction_152 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(152)

private:
    class MutatorASTConsumer_152 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_152(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
        std::vector<const clang::FunctionDecl *> cur_functions;
    };
};

//source file
#include "../include/mutate_function_return_type_with_decltype_152.h"

// ========================================================================================================
#define MUT152_OUTPUT 1

void MutatorFrontendAction_152::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->isDefaulted() || FD->isDeleted())
        return;
      if (FD->isMain())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isVoidType())
        return;
      if (FD->getReturnType()->isNullPtrType())
        return;
      if (FD->getReturnType()->isFunctionPointerType())
        return;
      if (FD->getReturnType()->isFunctionReferenceType())
        return;
      if (FD->getReturnType()->isFunctionType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionNoProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD->getReturnType()->isFunctionProtoType())
        return;
      if (FD