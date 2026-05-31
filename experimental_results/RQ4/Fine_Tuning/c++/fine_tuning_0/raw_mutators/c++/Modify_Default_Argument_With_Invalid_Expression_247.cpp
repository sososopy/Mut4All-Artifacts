//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Default_Argument_With_Invalid_Expression_247
 */ 
class MutatorFrontendAction_247 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(247)

private:
    class MutatorASTConsumer_247 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_247(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Modify_Default_Argument_With_Invalid_Expression_247.h"

// ========================================================================================================
#define MUT247_OUTPUT 1

void MutatorFrontendAction_247::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionWithDefaultArgs")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      
      if (FD->getNumParams() == 0)
        return;
      
      for (unsigned i = 0; i < FD->getNumParams(); ++i) {
        if (FD->getParamDecl(i)->hasDefaultArg()) {
          auto defaultArgRange = FD->getParamDecl(i)->getDefaultArgRange();
          auto defaultArgText = stringutils::rangetoStr(*(Result.SourceManager), defaultArgRange);
          
          // Modify the default argument to include a call to a non-existent function
          std::string mutatedDefaultArg = "non_existent_func()";
          
          Rewrite.ReplaceText(defaultArgRange, mutatedDefaultArg);
          break; // Only mutate the first default argument found
        }
      }
    }
}
  
void MutatorFrontendAction_247::MutatorASTConsumer_247::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionDecl(hasAnyParameter(hasDefaultArgument())).bind("FunctionWithDefaultArgs");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}