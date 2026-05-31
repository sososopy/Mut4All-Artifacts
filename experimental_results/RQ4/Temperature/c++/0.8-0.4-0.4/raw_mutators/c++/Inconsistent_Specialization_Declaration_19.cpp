//header file
#pragma once
#include "Mutator_base.h"

/**
 * inconsistent_specialization_declaration_19
 */ 
class MutatorFrontendAction_19 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(19)

private:
    class MutatorASTConsumer_19 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_19(Rewriter &R) : TheRewriter(R) {}
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
        std::set<std::string> specializedTemplates;
    };
};

//source file
#include "../include/inconsistent_specialization_declaration_19.h"

// ========================================================================================================
#define MUT19_OUTPUT 1

void MutatorFrontendAction_19::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CTD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("ClassTemplate")) {
        if (!CTD || !Result.Context->getSourceManager().isWrittenInMainFile(CTD->getLocation()))
            return;

        std::string templateName = CTD->getNameAsString();
        if (specializedTemplates.find(templateName) != specializedTemplates.end())
            return;

        specializedTemplates.insert(templateName);
        auto loc = CTD->getEndLoc();
        std::string specialization = "template<typename T> class " + templateName + "<T*> { /*mut19*/ };";
        Rewrite.InsertTextAfterToken(loc, "\n" + specialization);
    }
    else if (auto *CTSD = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("ClassTemplateSpecialization")) {
        if (!CTSD || !Result.Context->getSourceManager().isWrittenInMainFile(CTSD->getLocation()))
            return;

        std::string templateName = CTSD->getSpecializedTemplate()->getNameAsString();
        if (specializedTemplates.find(templateName) == specializedTemplates.end())
            return;

        auto loc = CTSD->getEndLoc();
        std::string conflictingSpecialization = "template<typename T> class " + templateName + "<T*> { /*mut19*/ };";
        Rewrite.InsertTextAfterToken(loc, "\n" + conflictingSpecialization);
    }
}

void MutatorFrontendAction_19::MutatorASTConsumer_19::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto classTemplateMatcher = classTemplateDecl().bind("ClassTemplate");
    auto classTemplateSpecializationMatcher = classTemplateSpecializationDecl().bind("ClassTemplateSpecialization");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(classTemplateMatcher, &callback);
    matchFinder.addMatcher(classTemplateSpecializationMatcher, &callback);
    matchFinder.matchAST(Context);
}