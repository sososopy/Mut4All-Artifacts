//header file
#pragma once
#include "Mutator_base.h"

/**
 * misuse_builtin_assumptions_15
 */ 
class MutatorFrontendAction_15 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(15)

private:
    class MutatorASTConsumer_15 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_15(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/misuse_builtin_assumptions_15.h"

// ========================================================================================================
#define MUT15_OUTPUT 1

void MutatorFrontendAction_15::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FC = Result.Nodes.getNodeAs<clang::CallExpr>("FuncCall")) {
      //Filter nodes in header files
      if (!FC || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FC->getExprLoc()))
        return;
      const FunctionDecl *callee = FC->getDirectCallee();
      if (!callee || !callee->isBuiltin()) return;

      // Check for built-in functions with pointer arguments
      if (callee->getName().startswith("__builtin_") && callee->getNumParams() > 0) {
        const ParmVarDecl *param = callee->getParamDecl(0);
        if (param->getType()->isPointerType()) {
          // Perform mutation on the source code text by applying string replacement
          std::string mutatedCall = callee->getNameAsString() + "(0";
          for (unsigned i = 1; i < FC->getNumArgs(); ++i) {
            mutatedCall += ", " + std::string(Lexer::getSourceText(CharSourceRange::getTokenRange(FC->getArg(i)->getSourceRange()), *Result.SourceManager, Result.Context->getLangOpts()));
          }
          mutatedCall += ")";
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(FC->getSourceRange()), mutatedCall);
        }
      }
    }
}
  
void MutatorFrontendAction_15::MutatorASTConsumer_15::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = callExpr(callee(functionDecl())).bind("FuncCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}