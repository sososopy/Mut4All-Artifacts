//header file
#pragma once
#include "Mutator_base.h"

/**
 * Mutate_Lambda_Parameters_104
 */ 
class MutatorFrontendAction_104 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(104)

private:
    class MutatorASTConsumer_104 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_104(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutate_Lambda_Parameters_104.h"

// ========================================================================================================
#define MUT104_OUTPUT 1

void MutatorFrontendAction_104::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambda")) {
      if (!LE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LE->getBeginLoc()))
        return;
      auto lambda_text = stringutils::rangetoStr(*(Result.SourceManager),
                                                 LE->getSourceRange());
      auto params = LE->getLambdaClass()->getLambdaCallOperator()->parameters();
      if (params.empty()) {
        lambda_text.insert(lambda_text.find(')'), "int x");
      } else {
        auto param = params[getrandom::getRandomIndex(params.size() - 1)];
        auto param_text = stringutils::rangetoStr(*(Result.SourceManager),
                                                  param->getSourceRange());
        llvm::outs() << "param_text: " << param_text << '\n';
        auto pos = lambda_text.find(param_text);
        while (pos != string::npos) {
          lambda_text.erase(pos, param_text.size());
          pos = lambda_text.find(param_text);
        }
      }
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(LE->getSourceRange()),
                          lambda_text);
    }
}
  
void MutatorFrontendAction_104::MutatorASTConsumer_104::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = lambdaExpr().bind("Lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}