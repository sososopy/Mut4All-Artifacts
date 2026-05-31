//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Decltype_Auto_With_Auto_In_Var_Decl_19
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
    };
};

//source file
#include "../include/Replace_Decltype_Auto_With_Auto_In_Var_Decl_19.h"

// ========================================================================================================
#define MUT19_OUTPUT 1

void MutatorFrontendAction_19::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("DecltypeAutoVar")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      auto DLtype = DL->getTypeSourceInfo();
      auto DLtypestr = stringutils::rangetoStr(*(Result.SourceManager),
                                               DLtype->getTypeLoc());
      llvm::outs() << DLtypestr << '\n';
      if (DLtypestr == "decltype(auto)") {
        DLtypestr = "auto";
        Rewrite.ReplaceText(DLtype->getTypeLoc().getSourceRange(), DLtypestr);
      }
    }
}
  
void MutatorFrontendAction_19::MutatorASTConsumer_19::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = varDecl().bind("DecltypeAutoVar");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}