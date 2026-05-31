//header file
#pragma once
#include "Mutator_base.h"

/**
 * Enclose_Assignment_Statement_With_Early_Return_220
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
#include "../include/enclose_assignment_statement_with_early_return_220.h"

// ========================================================================================================
#define MUT220_OUTPUT 1

void MutatorFrontendAction_220::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *AS = Result.Nodes.getNodeAs<clang::BinaryOperator>("Assign")) {
      if (!AS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     AS->getBeginLoc()))
        return;
      if (!AS->isAssignmentOp())
        return;
      if (!AS->getLHS()->getType()->isIntegerType())
        return;
      if (!AS->getRHS()->getType()->isIntegerType())
        return;
      auto lhs = stringutils::rangetoStr(*(Result.SourceManager),
                                         AS->getLHS()->getSourceRange());
      auto rhs = stringutils::rangetoStr(*(Result.SourceManager),
                                         AS->getRHS()->getSourceRange());
      auto replacement = "/*mut220*/({" + lhs + "=" + rhs + ";if(" + lhs +
                         ") return;})";
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(AS->getSourceRange()),
                          replacement);
    }
}
  
void MutatorFrontendAction_220::MutatorASTConsumer_220::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = binaryOperator().bind("Assign");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}