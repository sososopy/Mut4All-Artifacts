//header file
#pragma once
#include "Mutator_base.h"

/**
 * Misconfigure_Concept_Requires_Clause_77
 */ 
class MutatorFrontendAction_77 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(77)

private:
    class MutatorASTConsumer_77 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_77(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/misconfigure_concept_requires_clause_77.h"

// ========================================================================================================
#define MUT77_OUTPUT 1

void MutatorFrontendAction_77::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *RC = Result.Nodes.getNodeAs<clang::RequiresExpr>("RequiresClause")) {
      if (!RC || !Result.Context->getSourceManager().isWrittenInMainFile(
                     RC->getLocation()))
        return;

      auto clauseRange = RC->getSourceRange();
      auto clauseText = stringutils::rangetoStr(*(Result.SourceManager), clauseRange);

      size_t pos = clauseText.find(";");
      if (pos != std::string::npos) {
          clauseText.erase(pos, 1); // Remove the semicolon to introduce syntax error
      }

      Rewrite.ReplaceText(CharSourceRange::getTokenRange(clauseRange), clauseText);
    }
}
  
void MutatorFrontendAction_77::MutatorASTConsumer_77::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = requiresExpr().bind("RequiresClause");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}