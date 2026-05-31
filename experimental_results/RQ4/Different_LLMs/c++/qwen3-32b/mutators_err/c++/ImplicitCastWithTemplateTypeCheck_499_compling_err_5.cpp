//header file
#pragma once
#include "Mutator_base.h"

/**
 * ImplicitCastWithTemplateTypeCheck_499
 */ 
class MutatorFrontendAction_499 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(499)

private:
    class MutatorASTConsumer_499 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_499(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/ImplicitCastWithTemplateTypeCheck_499.h"

// ========================================================================================================
#define MUT499_OUTPUT 1

void MutatorFrontendAction_499::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ifStmt = Result.Nodes.getNodeAs<clang::IfStmt>("ifStmt")) {
        if (!ifStmt || !Result.Context->getSourceManager().isWrittenInMainFile(ifStmt->getBeginLoc()))
            return;

        auto *parent = ifStmt->getParent();
        if (!parent || !isa<clang::CompoundStmt>(parent))
            return;
        auto *grandParent = parent->getParent();
        if (!grandParent || !isa<clang::FunctionDecl>(grandParent))
            return;
        auto *func = cast<clang::FunctionDecl>(grandParent);
        if (isa<clang::FunctionTemplateDecl>(func->getDeclContext()))
            return;

        SourceLocation funcStart = func->getBeginLoc();
        Rewrite.InsertText(funcStart, "template <typename T> ", true, true);

        SourceLocation ifStart = ifStmt->getBeginLoc();
        std::string typeofDecl = "__typeof__(sizeof(T) > 0) cond;\n";
        Rewrite.InsertText(ifStart, typeofDecl, true, true);

        const clang::Expr *condExpr = ifStmt->getCond();
        SourceLocation condStart = condExpr->getBeginLoc();
        SourceLocation condEnd = condExpr->getEndLoc();
        std::string newCond = "!cond";
        Rewrite.ReplaceText(clang::CharSourceRange::getTokenRange(condStart, condEnd), newCond);
    }
}

void MutatorFrontendAction_499::MutatorASTConsumer_499::HandleTranslationUnit(clang::ASTContext &Context) {
    clang::MatchFinder matchFinder;
    clang::ast_matchers::StatementMatcher matcher = clang::ast_matchers::ifStmt().bind("ifStmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}