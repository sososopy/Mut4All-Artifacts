//header file
#pragma once
#include "Mutator_base.h"

/**
 * invalid_conditional_operator_in_range_loop_669
 */ 
class MutatorFrontendAction_669 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(669)

private:
    class MutatorASTConsumer_669 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_669(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/invalid_conditional_operator_in_range_loop_669.h"

// ========================================================================================================
#define MUT669_OUTPUT 1

void MutatorFrontendAction_669::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FL = Result.Nodes.getNodeAs<clang::CXXForRangeStmt>("ForLoop")) {
      if (!FL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FL->getBeginLoc()))
        return;

      auto rangeExpr = FL->getRangeInit();
      if (!rangeExpr)
        return;

      auto rangeSource = clang::Lexer::getSourceText(
          clang::CharSourceRange::getTokenRange(rangeExpr->getSourceRange()),
          *Result.SourceManager, clang::LangOptions());

      std::string mutatedRange = "condition ? " + rangeSource.str() + " : 3.14";
      Rewrite.ReplaceText(rangeExpr->getSourceRange(), mutatedRange);
    }
}
  
void MutatorFrontendAction_669::MutatorASTConsumer_669::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    StatementMatcher matcher = cxxForRangeStmt().bind("ForLoop");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}