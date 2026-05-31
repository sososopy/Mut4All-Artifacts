//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Scoped_Static_Var_With_Constexpr_448
 */ 
class MutatorFrontendAction_448 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(448)

private:
    class MutatorASTConsumer_448 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_448(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Scoped_Static_Var_With_Constexpr_448.h"

// ========================================================================================================
#define MUT448_OUTPUT 1

void MutatorFrontendAction_448::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("scopedStaticVar")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getBeginLoc()))
        return;
      if (!VD->isStaticLocal())
        return;
      auto type = stringutils::rangetoStr(*(Result.SourceManager),
                                          VD->getTypeSourceInfo()->getTypeLoc());
      auto name = VD->getNameAsString();
      auto init = stringutils::rangetoStr(*(Result.SourceManager),
                                          VD->getInit()->getSourceRange());
      string new_decl = "constexpr " + type + " " + name + " = " + init;
      llvm::outs() << new_decl << '\n';
      Rewrite.ReplaceText(VD->getSourceRange(), new_decl);
    }
}
  
void MutatorFrontendAction_448::MutatorASTConsumer_448::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = varDecl(hasAncestor(functionDecl())).bind("scopedStaticVar");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}