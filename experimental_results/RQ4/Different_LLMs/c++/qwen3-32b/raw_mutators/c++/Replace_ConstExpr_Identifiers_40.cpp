//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_ConstExpr_Identifiers_40
 */ 
class MutatorFrontendAction_40 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(40)

private:
    class MutatorASTConsumer_40 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_40(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_ConstExpr_Identifiers_40.h"

// ========================================================================================================
#define MUT40_OUTPUT 1

void MutatorFrontendAction_40::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DRE = Result.Nodes.getNodeAs<clang::DeclRefExpr>("const_expr_id")) {
        if (!DRE || !Result.Context->getSourceManager().isWrittenInMainFile(DRE->getLocation()))
            return;

        SourceLocation start = DRE->getNameInfo().getBeginLoc();
        SourceLocation end = DRE->getNameInfo().getEndLoc();
        if (start.isInvalid() || end.isInvalid())
            return;

        std::string originalName = DRE->getNameInfo().getName().getAsString();
        if (originalName.empty())
            return;

        std::string typoName = originalName;
        if (typoName.size() >= 2) {
            int index = getrandom::getRandomIndex(typoName.size() - 1);
            std::swap(typoName[index], typoName[index + 1]);
        }

        Rewrite.ReplaceText(SourceRange(start, end), typoName);
    }
}

void MutatorFrontendAction_40::MutatorASTConsumer_40::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = declRefExpr(ignoringImpCasts(constantExpr())).bind("const_expr_id");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}