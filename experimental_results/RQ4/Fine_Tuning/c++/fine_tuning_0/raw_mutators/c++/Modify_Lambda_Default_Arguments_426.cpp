//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Lambda_Default_Arguments_426
 */ 
class MutatorFrontendAction_426 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(426)

private:
    class MutatorASTConsumer_426 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_426(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Modify_Lambda_Default_Arguments_426.h"

// ========================================================================================================
#define MUT426_OUTPUT 1

void MutatorFrontendAction_426::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambdas")) {
      if (!LE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LE->getBeginLoc()))
        return;
      auto lambda = stringutils::rangetoStr(*(Result.SourceManager),
                                            LE->getSourceRange());
      auto params = LE->getLambdaClass()->getLambdaCallOperator()->parameters();
      for (auto param : params) {
        if (param->hasDefaultArg() == false) {
          lambda.insert(lambda.rfind(')'), "=0");
        } else {
          auto defaultarg = param->getDefaultArg();
          auto defaultarg_str = stringutils::rangetoStr(
              *(Result.SourceManager), defaultarg->getSourceRange());
          llvm::outs() << defaultarg_str << '\n';
          if (defaultarg_str == "0") {
            lambda.replace(lambda.find('='), 2, "=1");
          } else
            lambda.replace(lambda.find('='), 2, "=0");
        }
      }
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(LE->getSourceRange()), lambda);
    }
}
  
void MutatorFrontendAction_426::MutatorASTConsumer_426::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = lambdaExpr().bind("Lambdas");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}