//header file
#pragma once
#include "Mutator_base.h"

/**
 * Remove_Static_From_Consteval_Containing_Static_Var_325
 */ 
class MutatorFrontendAction_325 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(325)

private:
    class MutatorASTConsumer_325 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_325(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/remove_static_from_consteval_containing_static_var_325.h"

// ========================================================================================================
#define MUT325_OUTPUT 1

void MutatorFrontendAction_325::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("StaticVars")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (!DL->isStaticLocal())
        return;
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 DL->getSourceRange());
      llvm::outs() << declaration << '\n';
      declaration = stringutils::removeStatic(declaration);
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()),
                          declaration);
    }
  }

void MutatorFrontendAction_325::MutatorASTConsumer_325::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = varDecl(hasAncestor(functionDecl(isConsteval()))).bind("StaticVars");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}