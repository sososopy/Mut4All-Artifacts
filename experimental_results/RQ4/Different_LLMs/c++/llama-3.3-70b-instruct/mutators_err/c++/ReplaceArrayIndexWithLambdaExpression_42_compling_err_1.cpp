//header file
#pragma once
#include "Mutator_base.h"

/**
 * ReplaceArrayIndexWithLambdaExpression_42
 */ 
class MutatorFrontendAction_42 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(42)

private:
    class MutatorASTConsumer_42 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_42(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/ReplaceArrayIndexWithLambdaExpression_42.h"

// ========================================================================================================
#define MUT42_OUTPUT 1

void MutatorFrontendAction_42::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *AE = Result.Nodes.getNodeAs<clang::ArraySubscriptExpr>("ArrayExpr")) {
      //Filter nodes in header files
      if (!AE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     AE->getLocation()))
        return;
      //Get the source code text of target node
      auto index = AE->getIdx();
      auto indexText = stringutils::rangetoStr(*(Result.SourceManager), index->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      auto lambdaText = "[] { return " + indexText + "; }()";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(AE->getSourceRange()), lambdaText);
    }
}
  
void MutatorFrontendAction_42::MutatorASTConsumer_42::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    ExprMatcher matcher = arraySubscriptExpr().bind("ArrayExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}