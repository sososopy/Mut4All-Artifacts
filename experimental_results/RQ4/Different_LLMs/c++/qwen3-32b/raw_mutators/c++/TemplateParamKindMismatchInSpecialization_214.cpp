//header file
#pragma once
#include "Mutator_base.h"

/**
 * TemplateParamKindMismatchInSpecialization_214
 */ 
class MutatorFrontendAction_214 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(214)

private:
    class MutatorASTConsumer_214 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_214(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/TemplateParamKindMismatchInSpecialization_214.h"

// ========================================================================================================
#define MUT214_OUTPUT 1

void MutatorFrontendAction_214::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *partialSpec = Result.Nodes.getNodeAs<clang::ClassTemplatePartialSpecializationDecl>("partialSpec")) {
        if (!partialSpec || !Result.Context->getSourceManager().isWrittenInMainFile(partialSpec->getLocation()))
            return;

        auto *primaryTemplate = partialSpec->getSpecializedTemplate();
        if (!primaryTemplate)
            return;

        bool hasTypeParam = false;
        for (const auto &param : primaryTemplate->getTemplateParameters()->asArray()) {
            if (isa<clang::TemplateTypeParmDecl>(param)) {
                hasTypeParam = true;
                break;
            }
        }
        if (!hasTypeParam)
            return;

        for (const auto &param : partialSpec->getTemplateParameters()->asArray()) {
            if (isa<clang::TemplateTypeParmDecl>(param)) {
                SourceRange range = param->getSourceRange();
                std::string replacement = "int " + param->getNameAsString();
                Rewrite.ReplaceText(range, replacement);
            }
        }
    }
}

void MutatorFrontendAction_214::MutatorASTConsumer_214::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplatePartialSpecializationDecl().bind("partialSpec");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}