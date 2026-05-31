//header file
#pragma once
#include "Mutator_base.h"

/**
 * template_specialization_with_concepts_29
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
#include "../include/template_specialization_with_concepts_29.h"

// ========================================================================================================
#define MUT29_OUTPUT 1

void MutatorFrontendAction_29::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TC = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateClass")) {
        if (!TC || !Result.Context->getSourceManager().isWrittenInMainFile(TC->getLocation()))
            return;

        auto *conceptSpec = TC->getTemplateParameters()->getParam(0);
        if (auto *conceptType = llvm::dyn_cast<clang::TemplateTypeParmDecl>(conceptSpec)) {
            std::string conceptName = conceptType->getNameAsString();
            std::string className = TC->getNameAsString();

            std::string specialization = "template <" + conceptName + " T>\nstruct " + className + "<T> { /*mut29*/ };";
            SourceLocation insertLoc = TC->getEndLoc();
            Rewrite.InsertTextAfterToken(insertLoc, "\n" + specialization);
        }
    }
}

void MutatorFrontendAction_29::MutatorASTConsumer_29::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl(has(templateTypeParmDecl(isConcept()))).bind("TemplateClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}