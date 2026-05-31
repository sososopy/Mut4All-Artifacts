//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Export_Template_66
 */ 
class MutatorFrontendAction_66 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(66)

private:
    class MutatorASTConsumer_66 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_66(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Insert_Export_Template_66.h"

// ========================================================================================================
#define MUT66_OUTPUT 1

void MutatorFrontendAction_66::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::TemplateDecl>("template")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;
        SourceLocation loc = TD->getLocation();
        Rewrite.InsertText(loc, "export ", true, true);
    }
}

void MutatorFrontendAction_66::MutatorASTConsumer_66::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = clang::ast_matchers::templateDecl().bind("template");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}