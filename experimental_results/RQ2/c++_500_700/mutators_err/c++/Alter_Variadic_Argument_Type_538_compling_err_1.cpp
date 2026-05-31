//header file
#pragma once
#include "Mutator_base.h"

/**
 * alter_variadic_argument_type_538
 */ 
class MutatorFrontendAction_538 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(538)

private:
    class MutatorASTConsumer_538 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_538(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/alter_variadic_argument_type_538.h"

// ========================================================================================================
#define MUT538_OUTPUT 1

void MutatorFrontendAction_538::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("VariadicCall")) {
      //Filter nodes in header files
      if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CE->getBeginLoc()))
        return;

      for (unsigned i = 0; i < CE->getNumArgs(); ++i) {
        if (auto *VA = dyn_cast<clang::VAArgExpr>(CE->getArg(i))) {
          //Get the source code text of target node
          auto argType = VA->getType().getAsString();
          //Perform mutation on the source code text by applying string replacement
          std::string newArgType = argType + "*"; // Change to pointer type
          std::string replacement = "va_arg(" + VA->getSubExpr()->getSourceRange().getBegin().printToString(Result.Context->getSourceManager()) + ", " + newArgType + ")";
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(VA->getSourceRange(), replacement);
        }
      }
    }
}
  
void MutatorFrontendAction_538::MutatorASTConsumer_538::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = callExpr(hasAnyArgument(vaArgExpr())).bind("VariadicCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}