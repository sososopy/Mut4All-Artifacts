//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_template_concept_constraints_608
 */ 
class MutatorFrontendAction_608 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(608)

private:
    class MutatorASTConsumer_608 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_608(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_template_concept_constraints_608.h"

// ========================================================================================================
#define MUT608_OUTPUT 1

void MutatorFrontendAction_608::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TTD = Result.Nodes.getNodeAs<clang::TemplateTemplateParmDecl>("TemplateTemplateParam")) {
        if (!TTD || !Result.Context->getSourceManager().isWrittenInMainFile(TTD->getLocation()))
            return;

        // Get the source code text of target node
        auto templateText = stringutils::rangetoStr(*(Result.SourceManager), TTD->getSourceRange());

        // Perform mutation on the source code text by applying string replacement
        std::string newTemplateText;
        if (templateText.find("concept") != std::string::npos) {
            // Remove the concept constraint temporarily
            newTemplateText = std::regex_replace(templateText, std::regex("concept\\s+\\w+\\s*="), "typename");
        } else {
            // Re-introduce the concept with a different condition
            newTemplateText = std::regex_replace(templateText, std::regex("typename"), "concept C = (sizeof(T) > 1)");
        }

        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(TTD->getSourceRange()), newTemplateText);
    }
}
  
void MutatorFrontendAction_608::MutatorASTConsumer_608::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = templateTemplateParmDecl(hasType(templateTypeParmType())).bind("TemplateTemplateParam");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}