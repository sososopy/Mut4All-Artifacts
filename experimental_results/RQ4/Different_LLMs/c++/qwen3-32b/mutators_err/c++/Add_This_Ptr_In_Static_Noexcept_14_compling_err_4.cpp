//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_This_Ptr_In_Static_Noexcept_14
 */ 
class MutatorFrontendAction_14 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(14)

private:
    class MutatorASTConsumer_14 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_14(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
    };
};

//source file
#include "../include/Add_This_Ptr_In_Static_Noexcept_14.h"

// ========================================================================================================
#define MUT14_OUTPUT 1

void MutatorFrontendAction_14::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *method = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("method")) {
        if (!method || !Result.Context->getSourceManager().isWrittenInMainFile(
                       method->getLocation()))
            return;

        if (!method->isStatic()) return;

        Expr *noexceptExpr = method->getNoexcept();
        if (!noexceptExpr) return;

        SourceManager &SM = *Result.SourceManager;
        std::string exprText = stringutils::rangetoStr(SM, noexceptExpr->getSourceRange());

        std::string newExprText = "this->" + exprText;

        Rewrite.ReplaceText(noexceptExpr->getSourceRange(), newExprText);
    }
}
  
void MutatorFrontendAction_14::MutatorASTConsumer_14::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxMethodDecl(clang::ast_matchers::isStatic(), clang::ast_matchers::has(cxxNoexceptExpr())).bind("method");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}