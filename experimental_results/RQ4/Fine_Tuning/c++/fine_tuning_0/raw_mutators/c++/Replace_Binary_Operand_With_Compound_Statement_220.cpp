//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Binary_Operand_With_Compound_Statement_220
 */ 
class MutatorFrontendAction_220 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(220)

private:
    class MutatorASTConsumer_220 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_220(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Binary_Operand_With_Compound_Statement_220.h"

// ========================================================================================================
#define MUT220_OUTPUT 1

void MutatorFrontendAction_220::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *BO = Result.Nodes.getNodeAs<clang::BinaryOperator>("BinaryOp")) {
      if (!BO || !Result.Context->getSourceManager().isWrittenInMainFile(
                     BO->getBeginLoc()))
        return;
      if (!BO->getLHS()->getType()->isIntegerType() &&
          !BO->getLHS()->getType()->isEnumeralType())
        return;
      auto lhs = stringutils::rangetoStr(*(Result.SourceManager),
                                         BO->getLHS()->getSourceRange());
      auto rhs = stringutils::rangetoStr(*(Result.SourceManager),
                                         BO->getRHS()->getSourceRange());
      int dice = getrandom::getRandomIndex(1);
      auto name = (dice == 0) ? lhs : rhs;
      auto type = (dice == 0) ? BO->getLHS()->getType() : BO->getRHS()->getType();
      auto compound_stmt = "({ " + type.getAsString() + " " + name + "; " +
                           name + " = " + "0" + "; if (" + name + ") break; })";
      if (dice == 0) {
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(BO->getLHS()->getSourceRange()),
            compound_stmt);
      } else {
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(BO->getRHS()->getSourceRange()),
            compound_stmt);
      }
    }
}
  
void MutatorFrontendAction_220::MutatorASTConsumer_220::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = binaryOperator().bind("BinaryOp");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}