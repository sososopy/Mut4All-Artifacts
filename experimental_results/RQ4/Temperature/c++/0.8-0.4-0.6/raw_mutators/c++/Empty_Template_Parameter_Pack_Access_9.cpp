//header file
#pragma once
#include "Mutator_base.h"

/**
 * empty_template_parameter_pack_access_9
 */ 
class MutatorFrontendAction_9 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(9)

private:
    class MutatorASTConsumer_9 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_9(Rewriter &R) : TheRewriter(R) {}
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
        const clang::ClassTemplateDecl *currentTemplate = nullptr;
    };
};

//source file
#include "../include/empty_template_parameter_pack_access_9.h"

// ========================================================================================================
#define MUT9_OUTPUT 1

void MutatorFrontendAction_9::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CTD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("ClassTemplate")) {
        if (!CTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       CTD->getLocation()))
            return;

        currentTemplate = CTD;
    } else if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
        if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FTD->getLocation()))
            return;

        if (currentTemplate) {
            auto *TPL = FTD->getTemplateParameters();
            if (TPL->size() > 0 && isa<clang::TemplateTypeParmDecl>(TPL->getParam(0))) {
                std::string mutation = "template <unsigned Index> static auto get() -> ";
                mutation += TPL->getParam(0)->getNameAsString() + "...[Index] { return {}; }";
                mutation = "/*mut9*/" + mutation;
                Rewrite.InsertTextAfterToken(FTD->getEndLoc(), mutation);
            }
        }
    }
}

void MutatorFrontendAction_9::MutatorASTConsumer_9::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher classTemplateMatcher = classTemplateDecl().bind("ClassTemplate");
    DeclarationMatcher functionTemplateMatcher = functionTemplateDecl().bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(classTemplateMatcher, &callback);
    matchFinder.addMatcher(functionTemplateMatcher, &callback);
    matchFinder.matchAST(Context);
}