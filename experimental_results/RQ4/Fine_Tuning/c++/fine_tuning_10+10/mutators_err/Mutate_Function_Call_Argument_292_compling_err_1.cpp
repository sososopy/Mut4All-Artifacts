//header file
#pragma once
#include "Mutator_base.h"

/**
 * Mutate_Function_Call_Argument_292
 */ 
class MutatorFrontendAction_292 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(292)

private:
    class MutatorASTConsumer_292 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_292(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/mutate_function_call_argument_292.h"

// ========================================================================================================
#define MUT292_OUTPUT 1

void MutatorFrontendAction_292::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("FunctionCall")) {
      //Filter nodes in header files
      if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CE->getExprLoc()))
        return;

      //Get the source code text of target node
      auto args = CE->arguments();
      if (args.size() > 0) {
        auto arg = args[0];
        auto argText = Lexer::getSourceText(CharSourceRange::getTokenRange(arg->getSourceRange()), 
                                            *Result.SourceManager, 
                                            Result.Context->getLangOpts());
        //Perform mutation on the source code text by applying string replacement
        std::string mutatedArg = argText.str() + " / 0"; // Introducing division by zero
        Rewrite.ReplaceText(arg->getSourceRange(), mutatedArg);
      }
    }
}
  
void MutatorFrontendAction_292::MutatorASTConsumer_292::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = callExpr(callee(functionDecl())).bind("FunctionCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}