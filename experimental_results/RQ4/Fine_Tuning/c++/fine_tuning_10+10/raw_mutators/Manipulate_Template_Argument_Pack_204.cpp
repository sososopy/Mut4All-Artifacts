//header file
#pragma once
#include "Mutator_base.h"

/**
 * Manipulate_Template_Argument_Pack_204
 */ 
class MutatorFrontendAction_204 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(204)

private:
    class MutatorASTConsumer_204 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_204(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Manipulate_Template_Argument_Pack_204.h"

// ========================================================================================================
#define MUT204_OUTPUT 1

void MutatorFrontendAction_204::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("TemplateDecl")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;

        if (TD->getSpecializationKind() != TSK_ExplicitSpecialization)
            return;

        if (TD->getTemplateArgs().size() > 0) {
            std::string replacement = "Container<>::get<0>();";
            SourceLocation startLoc = TD->getBeginLoc();
            SourceLocation endLoc = TD->getEndLoc();
            Rewrite.ReplaceText(SourceRange(startLoc, endLoc), replacement);
        }
    }
}

void MutatorFrontendAction_204::MutatorASTConsumer_204::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateSpecializationDecl().bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}