//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Lambda_Capture_In_Immediate_Functions_78
 */ 
class MutatorFrontendAction_78 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(78)

private:
    class MutatorASTConsumer_78 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_78(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<std::string> localVars;
    };
};

//source file
#include "../include/modify_lambda_capture_in_immediate_functions_78.h"

// ========================================================================================================
#define MUT78_OUTPUT 1

void MutatorFrontendAction_78::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LambdaExpr = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambdaExpr")) {
      if (!LambdaExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LambdaExpr->getBeginLoc()))
        return;

      auto &CaptureList = LambdaExpr->capture_begin();
      std::set<std::string> capturedVars;

      for (auto it = CaptureList; it != LambdaExpr->capture_end(); ++it) {
          if (it->capturesVariable()) {
              capturedVars.insert(it->getCapturedVar()->getNameAsString());
          }
      }

      for (const auto &var : localVars) {
          if (capturedVars.find(var) == capturedVars.end()) {
              std::string newCapture = "[" + var + "]";
              Rewrite.InsertText(LambdaExpr->getBeginLoc(), newCapture, true, true);
              break;
          }
      }
    }
}
  
void MutatorFrontendAction_78::MutatorASTConsumer_78::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto lambdaMatcher = lambdaExpr(hasAncestor(callExpr())).bind("lambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(lambdaMatcher, &callback);
    matchFinder.matchAST(Context);
}