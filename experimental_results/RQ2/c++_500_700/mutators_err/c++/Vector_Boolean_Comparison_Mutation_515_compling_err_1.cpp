//header file
#pragma once
#include "Mutator_base.h"

/**
 * vector_boolean_comparison_mutation_515
 */ 
class MutatorFrontendAction_515 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(515)

private:
    class MutatorASTConsumer_515 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_515(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/vector_boolean_comparison_mutation_515.h"

// ========================================================================================================
#define MUT515_OUTPUT 1

void MutatorFrontendAction_515::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *BO = Result.Nodes.getNodeAs<clang::BinaryOperator>("VectorBoolComp")) {
      if (!BO || !Result.Context->getSourceManager().isWrittenInMainFile(
                     BO->getExprLoc()))
        return;

      auto lhsType = BO->getLHS()->getType();
      auto rhsType = BO->getRHS()->getType();

      if (lhsType->isVectorType() && rhsType->isVectorType() &&
          lhsType->getAs<clang::ExtVectorType>()->getElementType()->isBooleanType() &&
          rhsType->getAs<clang::ExtVectorType>()->getElementType()->isBooleanType()) {

        std::string newOp;
        switch (BO->getOpcode()) {
          case clang::BO_EQ:
          case clang::BO_NE:
            newOp = "&&";
            break;
          case clang::BO_LT:
          case clang::BO_GT:
          case clang::BO_LE:
          case clang::BO_GE:
            newOp = "||";
            break;
          default:
            return;
        }

        std::string lhsText = clang::Lexer::getSourceText(
            clang::CharSourceRange::getTokenRange(BO->getLHS()->getSourceRange()),
            *Result.SourceManager, Result.Context->getLangOpts()).str();

        std::string rhsText = clang::Lexer::getSourceText(
            clang::CharSourceRange::getTokenRange(BO->getRHS()->getSourceRange()),
            *Result.SourceManager, Result.Context->getLangOpts()).str();

        std::string mutatedExpr = "(" + lhsText + " " + newOp + " " + rhsText + ")";
        Rewrite.ReplaceText(BO->getSourceRange(), mutatedExpr);
      }
    }
}
  
void MutatorFrontendAction_515::MutatorASTConsumer_515::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    StatementMatcher matcher = binaryOperator(
        hasOperatorName("==").bind("VectorBoolComp"),
        hasLHS(hasType(extVectorType(hasElementType(booleanType())))),
        hasRHS(hasType(extVectorType(hasElementType(booleanType()))))
    ).bind("VectorBoolComp");

    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}