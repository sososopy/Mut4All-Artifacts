//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_default_parameter_to_template_528
 */ 
class MutatorFrontendAction_528 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(528)

private:
    class MutatorASTConsumer_528 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_528(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_default_parameter_to_template_528.h"

// ========================================================================================================
#define MUT528_OUTPUT 1

void MutatorFrontendAction_528::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionWithDefaultParam")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      for (unsigned i = 0; i < FD->getNumParams(); ++i) {
        auto *param = FD->getParamDecl(i);
        if (param->hasDefaultArg()) {
          //Get the source code text of target node
          auto defaultArg = param->getDefaultArg();
          if (defaultArg) {
            //Perform mutation on the source code text by applying string replacement
            std::string replacement = "T()";
            //Replace the original AST node with the mutated one
            Rewrite.ReplaceText(defaultArg->getSourceRange(), replacement);
            break;
          }
        }
      }
    }
}
  
void MutatorFrontendAction_528::MutatorASTConsumer_528::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(hasAnyParameter(hasDefaultArgument())).bind("FunctionWithDefaultParam");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}