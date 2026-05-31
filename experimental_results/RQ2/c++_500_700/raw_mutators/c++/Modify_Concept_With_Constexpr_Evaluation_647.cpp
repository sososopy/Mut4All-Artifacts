//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_concept_with_constexpr_evaluation_647
 */ 
class MutatorFrontendAction_647 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(647)

private:
    class MutatorASTConsumer_647 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_647(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_concept_with_constexpr_evaluation_647.h"

// ========================================================================================================
#define MUT647_OUTPUT 1

void MutatorFrontendAction_647::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CTD = Result.Nodes.getNodeAs<clang::ConceptDecl>("ConceptDecl")) {
      if (!CTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CTD->getLocation()))
        return;

      auto requiresExpr = Result.Nodes.getNodeAs<clang::RequiresExpr>("RequiresExpr");
      if (!requiresExpr)
        return;

      auto sourceRange = requiresExpr->getSourceRange();
      std::string originalCode = stringutils::rangetoStr(*(Result.SourceManager), sourceRange);

      std::string constexprCode = "constexpr bool is_valid = sizeof(T) > 0; is_valid;";
      std::string mutatedCode = originalCode + " " + constexprCode;

      Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), mutatedCode);
    }
}
  
void MutatorFrontendAction_647::MutatorASTConsumer_647::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = conceptDecl(hasRequiresExpr()).bind("ConceptDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}