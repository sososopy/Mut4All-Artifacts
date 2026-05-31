//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Constexpr_To_Function_Return_462
 */ 
class MutatorFrontendAction_462 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(462)

private:
    class MutatorASTConsumer_462 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_462(Rewriter &R) : TheRewriter(R) {}
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
        const FunctionDecl *last_func;
    };
};

//source file
#include "../include/add_constexpr_to_function_return_462.h"

// ========================================================================================================
#define MUT462_OUTPUT 1

void MutatorFrontendAction_462::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->isLambda())
        return;
      if (!FD->isConstexpr())
        return;
      last_func = FD;
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("Returns")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD != last_func)
        return;
      if (FD->getReturnType()->isVoidType()) {
        Rewrite.ReplaceText(FD->getReturnTypeSourceRange(), "int");
      } else {
        Rewrite.ReplaceText(FD->getReturnTypeSourceRange(), "double");
      }
    }
}

void MutatorFrontendAction_462::MutatorASTConsumer_462::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionDecl().bind("Functions");
    auto return_matcher = functionDecl().bind("Returns");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(return_matcher, &callback);
    matchFinder.matchAST(Context);
}