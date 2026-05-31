//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Operator_With_Complex_Expression_27
 */ 

class MutatorFrontendAction_27 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(27)
private:
    class MutatorASTConsumer_27 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_27(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/replace_operator_with_complex_expression_27.h"

// ========================================================================================================
#define MUT27_OUTPUT 1

void MutatorFrontendAction_27::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *BO = Result.Nodes.getNodeAs<clang::BinaryOperator>("BinaryOp")) {
      //Filter nodes in header files
      if (!BO || !Result.Context->getSourceManager().isWrittenInMainFile(
                     BO->getExprLoc()))
        return;

      //Get the source code text of target node
      auto lhs = stringutils::rangetoStr(*(Result.SourceManager), BO->getLHS()->getSourceRange());
      auto rhs = stringutils::rangetoStr(*(Result.SourceManager), BO->getRHS()->getSourceRange());

      //Perform mutation on the source code text by applying string replacement
      std::string complexExpr = "(((" + lhs + ") * (" + rhs + ")) + ((" + lhs + ") / (" + rhs + ")))";

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(BO->getSourceRange()), complexExpr);
    }
}
  
void MutatorFrontendAction_27::MutatorASTConsumer_27::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    auto matcher = binaryOperator(hasOperatorName("+")).bind("BinaryOp");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}