//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_concept_requirements_644
 */ 
class MutatorFrontendAction_644 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(644)

private:
    class MutatorASTConsumer_644 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_644(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_concept_requirements_644.h"

// ========================================================================================================
#define MUT644_OUTPUT 1

void MutatorFrontendAction_644::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CT = Result.Nodes.getNodeAs<clang::ConceptDecl>("Concepts")) {
      if (!CT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CT->getLocation()))
        return;

      auto requiresClause = CT->getRequiresClause();
      if (!requiresClause)
        return;

      auto sourceRange = requiresClause->getSourceRange();
      auto originalText = stringutils::rangetoStr(*(Result.SourceManager), sourceRange);

      std::string additionalRequirement = "requires std::is_constructible_v<T>;";
      std::string mutatedText = originalText + "\n" + additionalRequirement;

      Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), mutatedText);
    }
}
  
void MutatorFrontendAction_644::MutatorASTConsumer_644::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = conceptDecl(hasRequiresClause()).bind("Concepts");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}