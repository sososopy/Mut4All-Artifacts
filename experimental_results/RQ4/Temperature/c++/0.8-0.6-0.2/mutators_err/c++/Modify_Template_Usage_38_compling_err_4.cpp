// header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_template_usage_38
 */
class MutatorFrontendAction_38 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(38)

private:
    class MutatorASTConsumer_38 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_38(Rewriter &R) : TheRewriter(R) {}
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

// source file
#include "modify_template_usage_38.h"

// ========================================================================================================
#define MUT38_OUTPUT 1

void MutatorFrontendAction_38::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::MemberExpr>("templateMemberAccess")) {
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getExprLoc()))
            return;

        if (MT->hasTemplateKeyword()) return;

        SourceLocation LAngleLoc = MT->getLAngleLoc();
        if (LAngleLoc.isInvalid()) return;

        Rewrite.InsertTextBefore(LAngleLoc, "template ");
    }
}

void MutatorFrontendAction_38::MutatorASTConsumer_38::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = memberExpr(hasTemplateArgumentList()).bind("templateMemberAccess");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}