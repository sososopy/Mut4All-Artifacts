//header file
#pragma once
#include "Mutator_base.h"
#include "clang/AST/Expr.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Lex/Lexer.h"
#include "clang/AST/ExprConcepts.h"

/**
 * Modify_Concept_Constraint_109
 */ 
class MutatorFrontendAction_109 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(109)

private:
    class MutatorASTConsumer_109 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_109(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_concept_constraint_109.h"

using namespace clang;
using namespace clang::ast_matchers;

// ========================================================================================================
#define MUT109_OUTPUT 1

void MutatorFrontendAction_109::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CT = Result.Nodes.getNodeAs<ConceptSpecializationExpr>("ConceptExpr")) {
      if (!CT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CT->getBeginLoc()))
        return;

      auto sourceRange = CT->getSourceRange();
      auto originalText = Lexer::getSourceText(CharSourceRange::getTokenRange(sourceRange), 
                                               *Result.SourceManager, 
                                               Result.Context->getLangOpts()).str();

      std::string mutatedText = originalText;
      size_t pos = mutatedText.find("+");
      if (pos != std::string::npos) {
          mutatedText.replace(pos, 1, "/");
      }

      Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), mutatedText);
    }
}
  
void MutatorFrontendAction_109::MutatorASTConsumer_109::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = conceptSpecializationExpr().bind("ConceptExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}