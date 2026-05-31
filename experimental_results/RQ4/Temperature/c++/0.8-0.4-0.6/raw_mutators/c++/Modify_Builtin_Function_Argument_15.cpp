//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_builtin_function_argument_15
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
        const Expr *originalExpr;
    };
};

//source file
#include "../include/modify_builtin_function_argument_15.h"

// ========================================================================================================
#define MUT15_OUTPUT 1

void MutatorFrontendAction_15::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("builtinCall")) {
      //Filter nodes in header files
      if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CE->getExprLoc()))
        return;

      if (CE->getNumArgs() < 2)
        return;

      auto *firstArg = CE->getArg(0);
      auto *secondArg = CE->getArg(1);

      if (!firstArg->getType()->isPointerType())
        return;

      llvm::APSInt intValue;
      if (!secondArg->EvaluateAsInt(intValue, *Result.Context) || !intValue.isPowerOf2())
        return;

      //Perform mutation on the source code text by applying string replacement
      std::string replacementText = "0"; // Replace with an integer literal
      Rewrite.ReplaceText(firstArg->getSourceRange(), replacementText);
    }
}
  
void MutatorFrontendAction_15::MutatorASTConsumer_15::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = callExpr(callee(functionDecl(hasName("__builtin_assume_aligned")))).bind("builtinCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}