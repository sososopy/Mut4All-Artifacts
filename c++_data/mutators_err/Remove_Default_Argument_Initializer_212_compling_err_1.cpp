//header file
#pragma once
#include "Mutator_base.h"

/**
 * Remove_Default_Argument_Initializer_212
 */ 
class MutatorFrontendAction_212 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(212)

private:
    class MutatorASTConsumer_212 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_212(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/remove_default_argument_initializer_212.h"

// ========================================================================================================
#define MUT212_OUTPUT 1

void MutatorFrontendAction_212::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionWithDefaultArg")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      for (unsigned i = 0; i < FD->getNumParams(); ++i) {
        auto *param = FD->getParamDecl(i);
        if (param->hasDefaultArg()) {
          //Get the source code text of target node
          auto paramSourceRange = param->getSourceRange();
          auto defaultArgRange = param->getDefaultArgRange();
          auto defaultArgText = stringutils::rangetoStr(*(Result.SourceManager), defaultArgRange);
          //Perform mutation on the source code text by applying string replacement
          std::string newParamText = stringutils::rangetoStr(*(Result.SourceManager), paramSourceRange);
          size_t pos = newParamText.find(defaultArgText);
          if (pos != std::string::npos) {
            newParamText.erase(pos, defaultArgText.length());
            //Replace the original AST node with the mutated one
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(paramSourceRange), newParamText);
          }
          break; // Only mutate one default argument per function
        }
      }
    }
}
  
void MutatorFrontendAction_212::MutatorASTConsumer_212::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(hasAnyParameter(hasDefaultArg())).bind("FunctionWithDefaultArg");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}