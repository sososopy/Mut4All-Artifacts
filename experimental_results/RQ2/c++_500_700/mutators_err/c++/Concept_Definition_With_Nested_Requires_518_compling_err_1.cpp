//header file
#pragma once
#include "Mutator_base.h"

/**
 * concept_definition_with_nested_requires_518
 */ 
class MutatorFrontendAction_518 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(518)

private:
    class MutatorASTConsumer_518 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_518(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/concept_definition_with_nested_requires_518.h"

// ========================================================================================================
#define MUT518_OUTPUT 1

void MutatorFrontendAction_518::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CD = Result.Nodes.getNodeAs<clang::ConceptDecl>("ConceptDecl")) {
        if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       CD->getLocation()))
            return;

        auto sourceRange = CD->getSourceRange();
        auto sourceText = stringutils::rangetoStr(*(Result.SourceManager), sourceRange);

        // Perform mutation by introducing a nested malformed requires clause
        std::string nestedRequires = "requires requires { /* malformed */ ";
        size_t pos = sourceText.find("requires");
        if (pos != std::string::npos) {
            sourceText.insert(pos + 8, nestedRequires);
        }

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), sourceText);
    }
}
  
void MutatorFrontendAction_518::MutatorASTConsumer_518::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = conceptDecl().bind("ConceptDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}