//header file
#pragma once
#include "Mutator_base.h"

/**
 * Incomplete_Template_Parameter_In_Consteval_Function_306
 */ 
class MutatorFrontendAction_306 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(306)

private:
    class MutatorASTConsumer_306 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_306(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Incomplete_Template_Parameter_In_Consteval_Function_306.h"

// ========================================================================================================
#define MUT306_OUTPUT 1

void MutatorFrontendAction_306::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::TemplateDecl>("ConstevalTemplate")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;

        if (!TD->isFunctionTemplate())
            return;

        auto *FD = TD->getAsFunction();
        if (!FD || !FD->isConsteval())
            return;

        SourceRange templateRange = TD->getSourceRange();
        if (templateRange.isInvalid())
            return;

        int choice = getrandom::getRandomIndex(2);
        if (choice == 0) {
            Rewrite.ReplaceText(templateRange.getEnd(), 1, "");
        } else {
            Rewrite.ReplaceText(templateRange, "template <>");
        }
    }
}

void MutatorFrontendAction_306::MutatorASTConsumer_306::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = 
        templateDecl(
            functionTemplateDecl(
                hasDescendant(
                    functionDecl(
                        isConsteval()
                    ).bind("ConstevalFunction")
                )
            )
        ).bind("ConstevalTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}