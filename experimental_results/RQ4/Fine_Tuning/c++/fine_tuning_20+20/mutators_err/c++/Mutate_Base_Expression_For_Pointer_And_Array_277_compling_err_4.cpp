//header file
#pragma once
#include "Mutator_base.h"

/**
 * Mutate_Base_Expression_For_Pointer_And_Array_277
 */ 
class MutatorFrontendAction_277 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(277)

private:
    class MutatorASTConsumer_277 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_277(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        std::vector<const clang::Expr *> candidate_exprs;
    };
};

//source file
#include "../include/Mutate_Base_Expression_For_Pointer_And_Array_277.h"

// ========================================================================================================
#define MUT277_OUTPUT 1

void MutatorFrontendAction_277::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *EX = Result.Nodes.getNodeAs<clang::Expr>("Exprs")) {
      if (!EX || !Result.Context->getSourceManager().isWrittenInMainFile(
                     EX->getBeginLoc()))
        return;
      auto type = EX->getType();
      if (type->isPointerType() || type->isArrayType()) {
        candidate_exprs.push_back(EX);
      }
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::DeclRefExpr>("DeclRef")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getBeginLoc()))
        return;
      auto type = DL->getType();
      if (type->isPointerType() || type->isArrayType()) {
        candidate_exprs.push_back(DL);
      }
    } else if (auto *ML = Result.Nodes.getNodeAs<clang::MemberExpr>("Member")) {
      if (!ML || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ML->getBeginLoc()))
        return;
      auto type = ML->getType();
      if (type->isPointerType() || type->isArrayType()) {
        candidate_exprs.push_back(ML);
      }
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::DeclRefExpr>("DeclRef2")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getBeginLoc()))
        return;
      if (candidate_exprs.empty())
        return;
      auto index = getrandom::getRandomIndex(candidate_exprs.size() - 1);
      auto target = candidate_exprs[index];
      auto replacement = stringutils::exprToString(*(Result.Context), DL);
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(target->getSourceRange()), replacement);
    }
}

void MutatorFrontendAction_277::MutatorASTConsumer_277::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto expr_matcher = expr().bind("Exprs");
    auto declref_matcher = declRefExpr().bind("DeclRef");
    auto member_matcher = memberExpr().bind("Member");
    auto declref_matcher2 = declRefExpr().bind("DeclRef2");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(expr_matcher, &callback);
    matchFinder.addMatcher(declref_matcher, &callback);
    matchFinder.addMatcher(member_matcher, &callback);
    matchFinder.addMatcher(declref_matcher2, &callback);
    matchFinder.matchAST(Context);
}