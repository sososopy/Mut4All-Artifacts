//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Decltype_Auto_With_Auto_For_Function_Template_242
 */ 
class MutatorFrontendAction_242 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(242)

private:
    class MutatorASTConsumer_242 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_242(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Decltype_Auto_With_Auto_For_Function_Template_242.h"

// ========================================================================================================
#define MUT242_OUTPUT 1

void MutatorFrontendAction_242::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::DeclRefExpr>("DeclRef")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->getType()->isFunctionPointerType() == false)
        return;
      auto DLtype = DL->getType();
      auto DLtypestr = DLtype.getAsString();
      llvm::outs() << DLtypestr << '\n';
      if (DLtypestr.find("decltype") == std::string::npos)
        return;
      DLtypestr.replace(DLtypestr.find("decltype(auto)"), 14, "auto");
      llvm::outs() << DLtypestr << '\n';
      Rewrite.ReplaceText(DL->getBeginLoc(), 14, "auto");
    }
}
  
void MutatorFrontendAction_242::MutatorASTConsumer_242::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = declRefExpr().bind("DeclRef");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}