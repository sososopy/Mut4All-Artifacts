//header file
#pragma once
#include "Mutator_base.h"

/**
 * ConditionalExpressionArrayMutation_498
 */ 
class MutatorFrontendAction_498 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(498)

private:
    class MutatorASTConsumer_498 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_498(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::ArraySubscriptExpr *> arrayExprs;
    };
};

//source file
#include "../include/Mutator_ConditionalExpressionArrayMutation_498.h"

// ========================================================================================================
#define MUT498_OUTPUT 1

void MutatorFrontendAction_498::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CE = Result.Nodes.getNodeAs<clang::ConditionalOperator>(("ConditionalExpression"))) {
      //Filter nodes in header files
      if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CE->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto cond = stringutils::rangetoStr(*(Result.SourceManager), CE->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      if (CE->getLHS()->getType()->isArrayType() && CE->getRHS()->getType()->isArrayType()) {
        auto lhsType = CE->getLHS()->getType()->getArrayElementType()->getAsString();
        auto rhsType = CE->getRHS()->getType()->getArrayElementType()->getAsString();
        if (lhsType != rhsType) {
          // Replace the type of one of the arrays with a different type
          auto mutatedCond = cond;
          mutatedCond.replace(mutatedCond.find(lhsType), lhsType.length(), rhsType);
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(CE->getSourceRange()), mutatedCond);
        }
      } else if (CE->getLHS()->getType()->isArrayType()) {
        // If only one of the operands is an array, introduce a new array type
        auto arrayType = CE->getLHS()->getType()->getArrayElementType()->getAsString();
        auto mutatedCond = cond;
        mutatedCond.replace(mutatedCond.find(":"), 1, ": " + arrayType + " c[] = {0};");
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(CE->getSourceRange()), mutatedCond);
      } else if (CE->getRHS()->getType()->isArrayType()) {
        // If only one of the operands is an array, introduce a new array type
        auto arrayType = CE->getRHS()->getType()->getArrayElementType()->getAsString();
        auto mutatedCond = cond;
        mutatedCond.replace(mutatedCond.find(":"), 1, ": " + arrayType + " c[] = {0};");
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(CE->getSourceRange()), mutatedCond);
      }
    }
}

void MutatorFrontendAction_498::MutatorASTConsumer_498::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = conditionalOperator().bind("ConditionalExpression");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}