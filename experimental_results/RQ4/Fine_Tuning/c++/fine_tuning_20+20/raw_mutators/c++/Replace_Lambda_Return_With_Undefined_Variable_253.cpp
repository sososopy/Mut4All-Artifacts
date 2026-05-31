//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Lambda_Return_With_Undefined_Variable_253
 */ 
class MutatorFrontendAction_253 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(253)

private:
    class MutatorASTConsumer_253 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_253(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::LambdaExpr *> cur_lambdas;
    };
};

//source file
#include "../include/Replace_Lambda_Return_With_Undefined_Variable_253.h"

// ========================================================================================================
#define MUT253_OUTPUT 1

void MutatorFrontendAction_253::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LB = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambdas")) {
      if (!LB || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LB->getBeginLoc()))
        return;
      if (LB->hasExplicitParameters())
        return;
      cur_lambdas.push_back(LB);
    } else if (auto *RT = Result.Nodes.getNodeAs<clang::ReturnStmt>("Returns")) {
      if (!RT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     RT->getBeginLoc()))
        return;
      if (cur_lambdas.empty())
        return;
      auto cur_lambda = cur_lambdas.back();
      if (!RT->getBeginLoc().isMacroID() && RT->getBeginLoc() > cur_lambda->getBeginLoc() &&
          RT->getEndLoc() < cur_lambda->getEndLoc()) {
        auto content =
            stringutils::rangetoStr(*(Result.SourceManager), RT->getSourceRange());
        string target = "/*mut253*/return some_undefined_variable";
        Rewrite.ReplaceText(RT->getBeginLoc(), content.length(), target);
      }
    }
}
  
void MutatorFrontendAction_253::MutatorASTConsumer_253::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto lambda_matcher = lambdaExpr().bind("Lambdas");
    auto return_matcher = returnStmt().bind("Returns");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(lambda_matcher, &callback);
    matchFinder.addMatcher(return_matcher, &callback);
    matchFinder.matchAST(Context);
}