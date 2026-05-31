//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_default_template_argument_602
 */ 
class MutatorFrontendAction_602 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(602)

private:
    class MutatorASTConsumer_602 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_602(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_default_template_argument_602.h"

// ========================================================================================================
#define MUT602_OUTPUT 1

void MutatorFrontendAction_602::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("TemplateFunc")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;

        if (TD->getTemplateParameters()->size() > 0) {
            for (auto *param : *TD->getTemplateParameters()) {
                if (auto *typeParam = llvm::dyn_cast<clang::TemplateTypeParmDecl>(param)) {
                    if (typeParam->hasDefaultArgument()) {
                        auto defaultArg = typeParam->getDefaultArgument();
                        if (defaultArg.getAsString() == "int") {
                            SourceLocation startLoc = defaultArg.getSourceRange().getBegin();
                            SourceLocation endLoc = defaultArg.getSourceRange().getEnd();
                            Rewrite.ReplaceText(SourceRange(startLoc, endLoc), "double");
                        }
                    }
                }
            }
        }
    }
}

void MutatorFrontendAction_602::MutatorASTConsumer_602::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl().bind("TemplateFunc");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}