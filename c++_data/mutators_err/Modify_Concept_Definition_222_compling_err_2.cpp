//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Concept_Definition_222
 */ 
class MutatorFrontendAction_222 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(222)

private:
    class MutatorASTConsumer_222 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_222(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::ConceptDecl *> existingConcepts;
    };
};

//source file
#include "../include/Modify_Concept_Definition_222.h"
#include "clang/ASTMatchers/ASTMatchers.h"

using namespace clang;
using namespace clang::ast_matchers;

// ========================================================================================================
#define MUT222_OUTPUT 1

void MutatorFrontendAction_222::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CD = Result.Nodes.getNodeAs<clang::ConceptDecl>("Concepts")) {
        if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(CD->getLocation()))
            return;

        // Record existing concepts for potential use in mutation
        existingConcepts.push_back(CD);

        // Get the source code text of target node
        auto conceptText = stringutils::rangetoStr(*(Result.SourceManager), CD->getSourceRange());

        // Perform mutation on the source code text by applying string replacement
        if (!existingConcepts.empty()) {
            auto randomIndex = getrandom::getRandomIndex(existingConcepts.size() - 1);
            auto chosenConcept = existingConcepts[randomIndex]->getNameAsString();
            std::string mutatedExpression = "/*mut222*/" + chosenConcept + " && false";
            auto pos = conceptText.find('=');
            if (pos != std::string::npos) {
                conceptText.replace(pos + 1, std::string::npos, mutatedExpression);
            }
        }

        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(CD->getSourceRange()), conceptText);
    }
}

void MutatorFrontendAction_222::MutatorASTConsumer_222::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = conceptDecl().bind("Concepts");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}