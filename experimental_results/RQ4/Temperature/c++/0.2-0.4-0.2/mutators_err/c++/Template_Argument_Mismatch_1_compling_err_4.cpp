//header file
#pragma once
#include "Mutator_base.h"

/**
 * template_argument_mismatch_1
 */ 
class MutatorFrontendAction_1 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(1)

private:
    class MutatorASTConsumer_1 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_1(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/template_argument_mismatch_1.h"

// ========================================================================================================
#define MUT1_OUTPUT 1

void MutatorFrontendAction_1::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CallExpr = Result.Nodes.getNodeAs<clang::CallExpr>("callExpr")) {
        if (!CallExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                CallExpr->getBeginLoc()))
            return;

        if (auto *TemplateArgs = CallExpr->getDirectCallee()->getTemplateSpecializationArgs()) {
            std::string newTemplateArgs;
            for (unsigned i = 0; i < TemplateArgs->size(); ++i) {
                if (i == 0) {
                    newTemplateArgs += "double";
                } else {
                    newTemplateArgs += ", double";
                }
            }

            auto BeginLoc = TemplateArgs->getLAngleLoc();
            auto EndLoc = TemplateArgs->getRAngleLoc();
            Rewrite.ReplaceText(SourceRange(BeginLoc, EndLoc), newTemplateArgs);
        }
    }
}

void MutatorFrontendAction_1::MutatorASTConsumer_1::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = callExpr(callee(functionDecl(isTemplateInstantiation()))).bind("callExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}