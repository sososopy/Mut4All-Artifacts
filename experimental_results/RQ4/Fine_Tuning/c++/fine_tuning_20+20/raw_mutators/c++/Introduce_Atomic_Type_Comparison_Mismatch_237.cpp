//header file
#pragma once
#include "Mutator_base.h"

/**
 * Introduce_Atomic_Type_Comparison_Mismatch_237
 */ 
class MutatorFrontendAction_237 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(237)

private:
    class MutatorASTConsumer_237 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_237(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::VarDecl *> atomic_vars;
    };
};

//source file
#include "../include/introduce_atomic_type_comparison_mismatch_237.h"

// ========================================================================================================
#define MUT237_OUTPUT 1

void MutatorFrontendAction_237::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("AtomicVars")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->getType().getAsString().find("atomic") != string::npos) {
        atomic_vars.push_back(DL);
      }
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::BinaryOperator>(
                   "CompareExpr")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getBeginLoc()))
        return;
      if (DL->isComparisonOp() == false)
        return;
      auto lhs_type = DL->getLHS()->getType().getAsString();
      auto rhs_type = DL->getRHS()->getType().getAsString();
      if (lhs_type.find("atomic") != string::npos &&
          rhs_type.find("atomic") != string::npos)
        return;
      if (lhs_type.find("atomic") == string::npos &&
          rhs_type.find("atomic") == string::npos)
        return;
      auto lhs = DL->getLHS();
      auto rhs = DL->getRHS();
      if (lhs_type.find("atomic") != string::npos) {
        if (rhs_type.find("atomic") == string::npos) {
          for (std::size_t j = 0; j < atomic_vars.size(); ++j) {
            if (DL->getType().getAsString() ==
                atomic_vars[j]->getType().getAsString()) {
              auto name = atomic_vars[j]->getNameAsString();
              Rewrite.ReplaceText(rhs->getSourceRange(), name);
              return;
            }
          }
        }
      } else if (rhs_type.find("atomic") != string::npos) {
        if (lhs_type.find("atomic") == string::npos) {
          for (std::size_t j = 0; j < atomic_vars.size(); ++j) {
            if (DL->getType().getAsString() ==
                atomic_vars[j]->getType().getAsString()) {
              auto name = atomic_vars[j]->getNameAsString();
              Rewrite.ReplaceText(lhs->getSourceRange(), name);
              return;
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_237::MutatorASTConsumer_237::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto atomic_matcher = varDecl().bind("AtomicVars");
    auto compare_matcher = binaryOperator().bind("CompareExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(atomic_matcher, &callback);
    matchFinder.addMatcher(compare_matcher, &callback);
    matchFinder.matchAST(Context);
}