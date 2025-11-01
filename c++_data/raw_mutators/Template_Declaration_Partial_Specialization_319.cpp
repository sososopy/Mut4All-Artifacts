//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Declaration_Partial_Specialization_319
 */ 
class MutatorFrontendAction_319 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(319)

private:
    class MutatorASTConsumer_319 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_319(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/template_declaration_partial_specialization_319.h"

// ========================================================================================================
#define MUT319_OUTPUT 1

void MutatorFrontendAction_319::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateClass")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;

        auto &SM = Rewrite.getSourceMgr();
        auto &LangOpts = Rewrite.getLangOpts();
        auto Range = TD->getSourceRange();

        std::string OriginalTemplate = Lexer::getSourceText(CharSourceRange::getTokenRange(Range), SM, LangOpts).str();
        
        std::string TemplateName = TD->getNameAsString();
        std::string PartialSpecialization = "template <typename T> class " + TemplateName + "<T, true>;";

        std::string MutatedTemplate = OriginalTemplate + "\n/*mut319*/" + PartialSpecialization;

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(Range), MutatedTemplate);
    }
}

void MutatorFrontendAction_319::MutatorASTConsumer_319::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl().bind("TemplateClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}