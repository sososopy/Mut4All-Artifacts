//header file
#pragma once
#include "Mutator_base.h"

/**
 * Swap_Template_Arg_Pack_And_Arg_3
 */ 
class MutatorFrontendAction_{{bugno}} : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER({{bugno}})

private:
    class MutatorASTConsumer_{{bugno}} : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_{{bugno}}(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Swap_Template_Arg_Pack_And_Arg_3.h"

// ========================================================================================================
#define MUT{{bugno}}_OUTPUT 1

void MutatorFrontendAction_{{bugno}}::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TSL = Result.Nodes.getNodeAs<clang::TemplateSpecializationTypeLoc>("typeLoc")) {
        if (!Result.Context->getSourceManager().isWrittenInMainFile(TSL->getBeginLoc()))
            return;

        const auto &Args = TSL->getTemplateArgs();
        if (Args.size() < 2)
            return;

        if (Args[0].getKind() != clang::TemplateArgument::PackExpansion)
            return;

        SourceRange firstArgRange = Args[0].getSourceRange();
        SourceRange secondArgRange = Args[1].getSourceRange();

        std::string firstArgText = Rewrite.getRangeText(firstArgRange);
        std::string secondArgText = Rewrite.getRangeText(secondArgRange);

        Rewrite.ReplaceText(firstArgRange, secondArgText);
        Rewrite.ReplaceText(secondArgRange, firstArgText);
    }
}

void MutatorFrontendAction_{{bugno}}::MutatorASTConsumer_{{bugno}}::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = typeLoc(templateSpecializationTypeLoc()).bind("typeLoc");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}