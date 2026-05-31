//header file
#pragma once
#include "Mutator_base.h"

/**
 * Parenthesize_Initializer_List_Macros_317
 */ 
class MutatorFrontendAction_317 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(317)

private:
    class MutatorASTConsumer_317 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_317(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Parenthesize_Initializer_List_Macros_317.h"

// ========================================================================================================
#define MUT317_OUTPUT 1

void MutatorFrontendAction_317::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CallExpr>("MacroCall")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      if (!MT->getDirectCallee()->isDefined())
        return;
      if (!MT->getDirectCallee()->isVariadic())
        return;
      auto args = MT->getArgs();
      for (auto arg : args) {
        if (arg->getStmtClass() == Stmt::InitListExprClass) {
          auto text = stringutils::rangetoStr(
              *(Result.SourceManager), arg->getSourceRange());
          text = "((" + text + "))";
          Rewrite.ReplaceText(
              CharSourceRange::getTokenRange(arg->getSourceRange()), text);
        }
      }
    }
}
  
void MutatorFrontendAction_317::MutatorASTConsumer_317::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = callExpr().bind("MacroCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}