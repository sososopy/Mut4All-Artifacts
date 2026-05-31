//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Func_Template_Specialization_With_Lambda_19
 */ 
class MutatorFrontendAction_19 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(19)

private:
    class MutatorASTConsumer_19 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_19(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result);
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Mutator_Replace_Func_Template_Specialization_With_Lambda_19.h"

// ========================================================================================================
#define MUT19_OUTPUT 1

void MutatorFrontendAction_19::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("decltypeAutoVar")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (!VD->hasInit())
        return;
      auto *Init = VD->getInit();
      if (!Init)
        return;
      if (!VD->getType()->getContainedAutoType())
        return;
      if (!VD->getType()->isDecltypeType())
        return;
      auto *UE = dyn_cast<UnaryOperator>(Init);
      if (!UE)
        return;
      if (UE->getOpcode() != UO_AddrOf)
        return;
      auto *Ref = dyn_cast<DeclRefExpr>(UE->getSubExpr());
      if (!Ref)
        return;
    }
}