//header file
#pragma once
#include "Mutator_base.h"

/**
 * Move_Lambda_Capture_36
 */ 
class MutatorFrontendAction_36 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(36)

private:
    class MutatorASTConsumer_36 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_36(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Move_Lambda_Capture_36.h"

// ========================================================================================================
#define MUT36_OUTPUT 1

void MutatorFrontendAction_36::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LC = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambda")) {
      if (!LC || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LC->getBeginLoc()))
        return;
      if (LC->capture_size() == 0)
        return;
      auto LBD = LC->getLambdaClass();
      auto captures = LBD->captures();
      auto capture_begin = captures.begin();
      auto capture_end = captures.end();
      string capture_str = "[";
      for (auto it = capture_begin; it != capture_end; ++it) {
        capture_str +=
            it->getCapturedVar()->getNameAsString() + "=std::move(" +
            it->getCapturedVar()->getNameAsString() + ")";
        if (it != capture_end - 1)
          capture_str += ",";
      }
      capture_str += "]";
      auto capture_range = LC->getCaptureInit()->getSourceRange();
      llvm::outs() << capture_str << '\n';
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(capture_range), capture_str);
    }
}
  
void MutatorFrontendAction_36::MutatorASTConsumer_36::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = lambdaExpr().bind("Lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}