//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Fold_With_Nested_Requires_115
 */ 
class MutatorFrontendAction_115 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(115)

private:
    class MutatorASTConsumer_115 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_115(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Fold_With_Nested_Requires_115.h"

// ========================================================================================================
#define MUT115_OUTPUT 1

void MutatorFrontendAction_115::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *RE = Result.Nodes.getNodeAs<clang::RequiresExpr>("node")) {
        if (!RE || !Result.Context->getSourceManager().isWrittenInMainFile(RE->getLocation()))
            return;

        auto *ConstraintExpr = RE->getConstraintExpr();
        if (!ConstraintExpr)
            return;

        if (auto *FE = dyn_cast<clang::FoldExpr>(ConstraintExpr)) {
            SourceManager &SM = *Result.SourceManager;
            SourceLocation StartLoc = FE->getExprLoc();
            SourceLocation EndLoc = FE->getEndLoc();
            StringRef FoldText = SM.getCharacterData(StartLoc), SM.getCharacterData(EndLoc) - SM.getCharacterData(StartLoc));

            std::string NewText = "requires { (expr<0> ? requires { expr<1> ? ... : ... } : ...) }";

            Rewrite.ReplaceText(StartLoc, FoldText.size(), NewText);
        }
    }
}

void MutatorFrontendAction_115::MutatorASTConsumer_115::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = requiresExpr(hasConstraintExpr(foldExpr())).bind("node");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}