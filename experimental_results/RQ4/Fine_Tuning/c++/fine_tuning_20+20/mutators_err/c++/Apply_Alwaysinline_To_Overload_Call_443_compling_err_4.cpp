//header file
#pragma once
#include "Mutator_base.h"

/**
 * Apply_Alwaysinline_To_Overload_Call_443
 */ 
class MutatorFrontendAction_443 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(443)

private:
    class MutatorASTConsumer_443 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_443(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::FunctionDecl *> overloads;
    };
};

//source file
#include "../include/apply_alwaysinline_to_overload_call_443.h"

// ========================================================================================================
#define MUT443_OUTPUT 1

void MutatorFrontendAction_443::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Overloads")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getOverloadedOperator() != OverloadedOperatorKind::OO_None)
        return;
      overloads.push_back(FD);
    } else if (auto *CL = Result.Nodes.getNodeAs<clang::CallExpr>("Calls")) {
      if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CL->getBeginLoc()))
        return;
      if (overloads.empty())
        return;
      auto callee = CL->getDirectCallee();
      if (!callee)
        return;
      if (callee->getOverloadedOperator() != OverloadedOperatorKind::OO_None)
        return;
      if (callee->getPrimaryTemplate() == nullptr)
        return;
      auto callname = callee->getNameAsString();
      for (auto overload : overloads) {
        if (overload->getNameAsString() == callname) {
          Rewrite.ReplaceText(CL->getBeginLoc(), 0, "[[clang::always_inline]] ");
          break;
        }
      }
    }
}
  
void MutatorFrontendAction_443::MutatorASTConsumer_443::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto overload_matcher = functionDecl(isOverloaded()).bind("Overloads");
    auto call_matcher = callExpr().bind("Calls");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(overload_matcher, &callback);
    matchFinder.addMatcher(call_matcher, &callback);
    matchFinder.matchAST(Context);
}