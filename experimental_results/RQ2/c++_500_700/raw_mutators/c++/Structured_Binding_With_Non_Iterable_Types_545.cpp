//header file
#pragma once
#include "Mutator_base.h"

/**
 * structured_binding_with_non_iterable_types_545
 */ 
class MutatorFrontendAction_545 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(545)

private:
    class MutatorASTConsumer_545 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_545(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/structured_binding_with_non_iterable_types_545.h"

// ========================================================================================================
#define MUT545_OUTPUT 1

void MutatorFrontendAction_545::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("bindingDecl")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;

      if (auto *DRE = dyn_cast<DeclRefExpr>(VD->getInit()->IgnoreParenImpCasts())) {
        if (auto *FD = dyn_cast<FunctionDecl>(DRE->getDecl())) {
          if (FD->getNumParams() == 1) {
            auto paramType = FD->getParamDecl(0)->getType();
            if (paramType->isFundamentalType()) {
              auto funcName = FD->getNameAsString();
              std::string newCall = funcName + "(42)"; // Example with integer
              Rewrite.ReplaceText(DRE->getSourceRange(), newCall);
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_545::MutatorASTConsumer_545::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = varDecl(hasInitializer(declRefExpr(to(functionDecl())))).bind("bindingDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}