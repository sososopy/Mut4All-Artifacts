//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Requires_Clause_In_Lambda_With_Unrelated_Concept_444
 */ 
class MutatorFrontendAction_444 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(444)

private:
    class MutatorASTConsumer_444 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_444(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Requires_Clause_In_Lambda_With_Unrelated_Concept_444.h"

// ========================================================================================================
#define MUT444_OUTPUT 1

void MutatorFrontendAction_444::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("LambdaWithRequires")) {
      if (!LE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LE->getBeginLoc()))
        return;

      auto requiresClause = LE->getRequiresClause();
      if (!requiresClause)
        return;

      auto requiresText = stringutils::rangetoStr(
          *(Result.SourceManager), requiresClause->getSourceRange());

      auto concepts = Result.Context->getTranslationUnitDecl()->lookup(
          Result.Context->Idents.get("concept"));
      if (concepts.empty())
        return;

      auto conceptDecl = concepts.front();
      if (!conceptDecl)
        return;

      auto conceptName = conceptDecl->getNameAsString();
      auto newRequiresText = "requires " + conceptName;

      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(requiresClause->getSourceRange()),
          newRequiresText);
    }
}
  
void MutatorFrontendAction_444::MutatorASTConsumer_444::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = lambdaExpr(hasRequiresClause()).bind("LambdaWithRequires");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}