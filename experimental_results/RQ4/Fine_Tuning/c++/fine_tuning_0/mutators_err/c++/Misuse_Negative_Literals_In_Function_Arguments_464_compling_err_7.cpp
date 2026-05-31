//header file
#pragma once
#include "Mutator_base.h"

/**
 * Misuse_Negative_Literals_In_Function_Arguments_464
 */ 
class MutatorFrontendAction_464 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(464)

private:
    class MutatorASTConsumer_464 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_464(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Misuse_Negative_Literals_In_Function_Arguments_464.h"

// ========================================================================================================
#define MUT464_OUTPUT 1

void MutatorFrontendAction_464::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CallExpr>("FunctionCall")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      auto args = MT->getArgs();
      for (auto arg : args) {
        if (arg->getType()->isIntegerType()) {
          auto arg_str = stringutils::rangetoStr(*(Result.SourceManager),
                                                 arg->getSourceRange());
          llvm::outs() << "arg: " << arg_str << '\n';
          int choice = getrandom::getRandomIndex(1);
          if (choice) {
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(arg->getSourceRange()), "-255LL");
          }
        }
      }
    }
}
  
void MutatorFrontendAction_464::MutatorASTConsumer_464::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = callExpr().bind("FunctionCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}