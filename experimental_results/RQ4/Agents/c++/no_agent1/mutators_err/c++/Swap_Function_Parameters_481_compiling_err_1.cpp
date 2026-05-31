//header file
#pragma once
#include "Mutator_base.h"

/**
 * Swap_Function_Parameters_481
 */ 
class MutatorFrontendAction_481 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(481)

private:
    class MutatorASTConsumer_481 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_481(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Swap_Function_Parameters_481.h"

// ========================================================================================================
#define MUT481_OUTPUT 1

void MutatorFrontendAction_481::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (FD->getNumParams() < 2)
            return;

        auto *Param1 = FD->getParamDecl(0);
        auto *Param2 = FD->getParamDecl(1);

        if (!Param1 || !Param2)
            return;

        auto Param1Range = Param1->getSourceRange();
        auto Param2Range = Param2->getSourceRange();

        auto Param1Text = stringutils::rangetoStr(*(Result.SourceManager), Param1Range);
        auto Param2Text = stringutils::rangetoStr(*(Result.SourceManager), Param2Range);

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(Param1Range), Param2Text);
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(Param2Range), Param1Text);
    }
}

void MutatorFrontendAction_481::MutatorASTConsumer_481::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(hasParameter(0), hasParameter(1)).bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}