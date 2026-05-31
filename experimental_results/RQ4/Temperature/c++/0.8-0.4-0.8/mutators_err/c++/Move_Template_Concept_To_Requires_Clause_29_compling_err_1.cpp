//header file
#pragma once
#include "Mutator_base.h"

/**
 * move_template_concept_to_requires_clause_29
 */ 
class MutatorFrontendAction_29 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(29)

private:
    class MutatorASTConsumer_29 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_29(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_29.h"

// ========================================================================================================
#define MUT29_OUTPUT 1

void MutatorFrontendAction_29::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TD = Result.Nodes.getNodeAs<clang::TemplateTypeParmDecl>("templateParam")) {
        //Filter nodes in header files
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;

        //Get the source code text of target node
        auto templateDecl = TD->getTemplateDecl();
        if (!templateDecl)
            return;

        // Check if the template parameter has a concept
        if (TD->hasTypeConstraint()) {
            auto ConceptName = TD->getTypeConstraint()->getNameAsString();
            auto TemplateRange = templateDecl->getSourceRange();
            auto TemplateText = stringutils::rangetoStr(*(Result.SourceManager), TemplateRange);

            // Perform mutation on the source code text by applying string replacement
            std::string newTemplateText = TemplateText;
            size_t pos = newTemplateText.find(ConceptName + " ");
            if (pos != std::string::npos) {
                newTemplateText.replace(pos, ConceptName.length() + 1, "");
                newTemplateText += " requires " + ConceptName + "<" + TD->getNameAsString() + ">";
            }

            // Replace the original AST node with the mutated one
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(TemplateRange), newTemplateText);
        }
    }
}

void MutatorFrontendAction_29::MutatorASTConsumer_29::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = templateTypeParmDecl(hasTypeConstraint()).bind("templateParam");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}