//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Specialization_150
 */ 
class MutatorFrontendAction_150 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(150)

private:
    class MutatorASTConsumer_150 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_150(Rewriter &R) : TheRewriter(R) {}
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
        std::map<const clang::TemplateDecl*, std::vector<const clang::TemplateSpecializationType*>> templateSpecializations;
    };
};

//source file
#include "../include/modify_template_specialization_150.h"

// ========================================================================================================
#define MUT150_OUTPUT 1

void MutatorFrontendAction_150::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TS = Result.Nodes.getNodeAs<clang::TemplateSpecializationType>("TemplateSpec")) {
        if (!TS)
            return;

        const TemplateDecl *templateDecl = TS->getTemplateName().getAsTemplateDecl();
        if (!templateDecl)
            return;

        templateSpecializations[templateDecl].push_back(TS);

        // Perform mutation by altering the template argument
        if (templateSpecializations[templateDecl].size() == 1) {
            std::string mutatedSpec = "void(*)(int)";
            if (const auto *TST = Result.Nodes.getNodeAs<clang::TypeLoc>("TemplateSpecLoc")) {
                auto sourceRange = TST->getSourceRange();
                std::string originalText = stringutils::rangetoStr(*(Result.SourceManager), CharSourceRange::getTokenRange(sourceRange));
                std::string mutatedText = originalText;
                size_t start = mutatedText.find('<') + 1;
                size_t end = mutatedText.find('>');
                mutatedText.replace(start, end - start, mutatedSpec);

                Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), mutatedText);
            }
        }
    }
}

void MutatorFrontendAction_150::MutatorASTConsumer_150::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation.
    auto matcher = templateSpecializationType().bind("TemplateSpec");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}