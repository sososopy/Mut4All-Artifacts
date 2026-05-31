//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_lambda_return_type_628
 */ 
class MutatorFrontendAction_628 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(628)

private:
    class MutatorASTConsumer_628 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_628(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_lambda_return_type_628.h"

// ========================================================================================================
#define MUT628_OUTPUT 1

void MutatorFrontendAction_628::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LambdaExpr = Result.Nodes.getNodeAs<clang::LambdaExpr>("LambdaExpr")) {
      if (!LambdaExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LambdaExpr->getBeginLoc()))
        return;

      const auto *LambdaType = LambdaExpr->getLambdaClass()->getLambdaTypeInfo()->getType().getTypePtr();
      if (LambdaType && LambdaType->isDependentType()) {
        const auto *ReturnType = LambdaExpr->getCallOperator()->getReturnType().getTypePtr();
        if (ReturnType && ReturnType->isDependentType()) {
          auto LambdaSourceRange = LambdaExpr->getSourceRange();
          auto LambdaSourceText = stringutils::rangetoStr(*(Result.SourceManager), LambdaSourceRange);
          size_t pos = LambdaSourceText.find("-> decltype");
          if (pos != std::string::npos) {
            LambdaSourceText.replace(pos, LambdaSourceText.find('}', pos) - pos, "-> void");
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(LambdaSourceRange), LambdaSourceText);
          }
        }
      }
    }
}
  
void MutatorFrontendAction_628::MutatorASTConsumer_628::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = lambdaExpr(hasDescendant(decltypeType())).bind("LambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}