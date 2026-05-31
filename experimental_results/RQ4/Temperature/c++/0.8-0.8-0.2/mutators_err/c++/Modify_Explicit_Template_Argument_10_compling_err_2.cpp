//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_explicit_template_argument_10
 */ 
class MutatorFrontendAction_10 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(10)

private:
    class MutatorASTConsumer_10 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_10(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_explicit_template_argument_10.h"

// ========================================================================================================
#define MUT10_OUTPUT 1

void MutatorFrontendAction_10::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CallExpr = Result.Nodes.getNodeAs<clang::CallExpr>("TemplateFunctionCall")) {
        if (!CallExpr || !Result.Context->getSourceManager().isWrittenInMainFile(CallExpr->getExprLoc()))
            return;

        if (CallExpr->getNumTemplateArgs() > 0) {
            const auto *TemplateArgs = CallExpr->getTemplateArgs();
            if (!TemplateArgs)
                return;
            
            std::string newType = "float";
            const auto *OldArg = TemplateArgs->get(0).getAsType().getTypePtrOrNull();
            if (!OldArg)
                return;

            auto OldTypeSourceRange = OldArg->getLocallyUnqualifiedSingleStepDesugaredType()->getSourceRange();
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(OldTypeSourceRange), newType);
        }
    }
}

void MutatorFrontendAction_10::MutatorASTConsumer_10::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = callExpr(hasAnyTemplateArgument()).bind("TemplateFunctionCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}