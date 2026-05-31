//header file
#pragma once
#include "Mutator_base.h"

/**
 * Conditional_Expression_Mutator_456
 */ 
class MutatorFrontendAction_456 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(456)

private:
    class MutatorASTConsumer_456 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_456(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Conditional_Expression_Mutator_456.h"

// ========================================================================================================
#define MUT456_OUTPUT 1

void MutatorFrontendAction_456::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CE = Result.Nodes.getNodeAs<clang::ConditionalOperator>(("ConditionalExpression"))) {
      //Filter nodes in header files
      if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CE->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto condition = stringutils::rangetoStr(*(Result.SourceManager), CE->getCond()->getSourceRange());
      auto lhs = stringutils::rangetoStr(*(Result.SourceManager), CE->getLHS()->getSourceRange());
      auto rhs = stringutils::rangetoStr(*(Result.SourceManager), CE->getRHS()->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      //Replace the condition with a different boolean expression
      std::string newCondition = "true"; // or "false", or a variable, or a function call
      std::string mutatedExpression = newCondition + " ? " + lhs + " : " + rhs;
      //Swap the operands
      //std::string mutatedExpression = condition + " ? " + rhs + " : " + lhs;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(CE->getSourceRange()), mutatedExpression);
    }
}
  
void MutatorFrontendAction_456::MutatorASTConsumer_456::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    ExpressionMatcher matcher = conditionalOperator().bind("ConditionalExpression");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}