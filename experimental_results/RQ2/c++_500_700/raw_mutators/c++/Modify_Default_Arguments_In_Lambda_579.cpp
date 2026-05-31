//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_default_arguments_in_lambda_579
 */ 
class MutatorFrontendAction_579 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(579)

private:
    class MutatorASTConsumer_579 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_579(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_default_arguments_in_lambda_579.h"

// ========================================================================================================
#define MUT579_OUTPUT 1

void MutatorFrontendAction_579::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LambdaExpr = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambdaExpr")) {
      if (!LambdaExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LambdaExpr->getLocation()))
        return;

      for (auto *Param : LambdaExpr->getLambdaClass()->getLambdaCallOperator()->parameters()) {
        if (Param->hasDefaultArg()) {
          auto DefaultArg = Param->getDefaultArg();
          if (DefaultArg) {
            auto DefaultArgText = Lexer::getSourceText(CharSourceRange::getTokenRange(DefaultArg->getSourceRange()), 
                                                       Result.SourceManager, Result.Context->getLangOpts());
            std::string MutatedArg = "({ return 42; " + std::string(DefaultArgText) + "; })";
            Rewrite.ReplaceText(DefaultArg->getSourceRange(), MutatedArg);
          }
        }
      }
    }
}
  
void MutatorFrontendAction_579::MutatorASTConsumer_579::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = lambdaExpr(hasAnyParameter(hasDefaultArgument())).bind("lambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}