//header file
#pragma once
#include "Mutator_base.h"

/**
 * Incomplete_Concept_Syntax_224
 */ 
class MutatorFrontendAction_224 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(224)

private:
    class MutatorASTConsumer_224 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_224(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Incomplete_Concept_Syntax_224.h"

// ========================================================================================================
#define MUT224_OUTPUT 1

void MutatorFrontendAction_224::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CD = Result.Nodes.getNodeAs<clang::ConceptDecl>("concept")) {
        if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(CD->getLocation()))
            return;
        auto conceptRange = CD->getSourceRange();
        auto conceptText = stringutils::rangetoStr(*Result.SourceManager, conceptRange);
        size_t eqPos = conceptText.find('=');
        if (eqPos != string::npos) {
            string mutatedText = conceptText.substr(0, eqPos) + ";";
            Rewrite.ReplaceText(conceptRange, mutatedText);
        }
    }
    else if (auto *tparam = Result.Nodes.getNodeAs<clang::TemplateTypeParmDecl>("tparam")) {
        if (!tparam || !tparam->hasConstraint())
            return;
        if (!Result.Context->getSourceManager().isWrittenInMainFile(tparam->getLocation()))
            return;
        auto *TD = dyn_cast<clang::TemplateDecl>(tparam->getDeclContext());
        if (!TD)
            return;
        auto templateRange = TD->getSourceRange();
        auto templateText = stringutils::rangetoStr(*Result.SourceManager, templateRange);
        size_t closingAnglePos = templateText.rfind('>');
        if (closingAnglePos != string::npos) {
            string mutatedText = templateText.substr(0, closingAnglePos) + templateText.substr(closingAnglePos + 1);
            Rewrite.ReplaceText(templateRange, mutatedText);
        }
    }
}

void MutatorFrontendAction_224::MutatorASTConsumer_224::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher conceptMatcher = conceptDecl().bind("concept");
    DeclarationMatcher tparamMatcher = templateTypeParmDecl(hasConstraint()).bind("tparam");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(conceptMatcher, &callback);
    matchFinder.addMatcher(tparamMatcher, &callback);
    matchFinder.matchAST(Context);
}