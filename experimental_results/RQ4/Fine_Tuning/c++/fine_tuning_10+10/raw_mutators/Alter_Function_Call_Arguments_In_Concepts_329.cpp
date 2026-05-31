//header file
#pragma once
#include "Mutator_base.h"

/**
 * Alter_Function_Call_Arguments_In_Concepts_329
 */ 
class MutatorFrontendAction_329 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(329)

private:
    class MutatorASTConsumer_329 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_329(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/alter_function_call_arguments_in_concepts_329.h"

// ========================================================================================================
#define MUT329_OUTPUT 1

void MutatorFrontendAction_329::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("FunctionCall")) {
      //Filter nodes in header files
      if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CE->getExprLoc()))
        return;
      
      //Get the source code text of target node
      auto originalCall = stringutils::rangetoStr(*(Result.SourceManager), CE->getSourceRange());

      //Perform mutation on the source code text by applying string replacement
      // Introduce a new type that does not satisfy the concept
      std::string mutatedCall = originalCall;
      mutatedCall = "NonIncrementable y; " + mutatedCall;
      mutatedCall.insert(mutatedCall.find('(') + 1, "y");

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(CE->getSourceRange()), mutatedCall);
    }
}

void MutatorFrontendAction_329::MutatorASTConsumer_329::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = callExpr(callee(functionDecl(hasAnyParameter(hasType(templateTypeParmType()))))).bind("FunctionCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}