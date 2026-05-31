//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_static_member_function_usage_592
 */ 
class MutatorFrontendAction_592 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(592)

private:
    class MutatorASTConsumer_592 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_592(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_static_member_function_usage_592.h"

// ========================================================================================================
#define MUT592_OUTPUT 1

void MutatorFrontendAction_592::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("StaticMethod")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      
      if (MT->isStatic() && MT->doesThisDeclarationHaveABody()) {
        auto noexceptExpr = MT->getNoexceptExpr();
        if (noexceptExpr && noexceptExpr->isValueDependent()) {
          std::string modifiedNoexcept = "noexcept(true)";
          Rewrite.ReplaceText(noexceptExpr->getSourceRange(), modifiedNoexcept);
        }
      }
    }
}
  
void MutatorFrontendAction_592::MutatorASTConsumer_592::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = methodDecl(isStaticStorageClass(), isTemplateInstantiation()).bind("StaticMethod");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}