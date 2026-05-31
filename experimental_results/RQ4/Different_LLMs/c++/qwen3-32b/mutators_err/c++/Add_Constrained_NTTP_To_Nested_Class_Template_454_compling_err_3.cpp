//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Constrained_NTTP_To_Nested_Class_Template_454
 */ 
class MutatorFrontendAction_454 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(454)

private:
    class MutatorASTConsumer_454 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_454(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite), conceptInserted(false) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        bool conceptInserted;
        // Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Add_Constrained_NTTP_To_Nested_Class_Template_454.h"

// ========================================================================================================
#define MUT454_OUTPUT 1

void MutatorFrontendAction_454::Callback::run(const MatchFinder::MatchResult &Result) {
    // Check if the matched node is a concept declaration named 'C'
    if (auto *TD = Result.Nodes.getNodeAs<clang::TemplateDecl>("concept")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;
        if (isa<clang::ConceptDecl>(TD)) {
            conceptInserted = true; // Mark that the concept already exists
            return;
        }
    }

    // Check if the matched node is the inner class template
    if (auto *CT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("innerTemplate")) {
        if (!CT || !Result.Context->getSourceManager().isWrittenInMainFile(CT->getLocation()))
            return;

        // If the concept hasn't been inserted yet, insert it at the beginning
        if (!conceptInserted) {
            // Insert the concept declaration at the start of the translation unit
            SourceLocation startLoc = CT->getBeginLoc();
            Rewrite.InsertText(startLoc, "template <class> concept C = true;\n", true, true);
            conceptInserted = true;
        }

        // Get the source range of the template parameters
        SourceRange templateParamsRange = CT->getTemplateParameters()->getSourceRange();
        std::string newParam = ", C auto";
        // Replace the end of the template parameters with the new parameter
        Rewrite.InsertTextAfterToken(templateParamsRange.getEnd(), newParam);
    }
}

void MutatorFrontendAction_454::MutatorASTConsumer_454::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Match the inner class template inside an outer class template
    DeclarationMatcher innerTemplateMatcher = classTemplateDecl(
        isDefinition(),
        hasParent(classTemplateDecl()))
        .bind("innerTemplate");
    // Match any concept declaration named 'C'
    DeclarationMatcher conceptMatcher = conceptDecl(
        hasName("C")).bind("concept");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(innerTemplateMatcher, &callback);
    matchFinder.addMatcher(conceptMatcher, &callback);
    matchFinder.matchAST(Context);
}