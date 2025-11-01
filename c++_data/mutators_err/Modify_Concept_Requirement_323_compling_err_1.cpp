//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Concept_Requirement_323
 */ 
class MutatorFrontendAction_323 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(323)

private:
    class MutatorASTConsumer_323 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_323(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Modify_Concept_Requirement_323.h"

// ========================================================================================================
#define MUT323_OUTPUT 1

void MutatorFrontendAction_323::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ReqExpr = Result.Nodes.getNodeAs<clang::ConceptSpecializationExpr>("ConceptExpr")) {
      if (!ReqExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ReqExpr->getLocation()))
        return;

      auto sourceRange = ReqExpr->getSourceRange();
      auto sourceText = stringutils::rangetoStr(*(Result.SourceManager), sourceRange);

      // Perform mutation on the source code text by applying string replacement
      size_t autoPos = sourceText.find("auto");
      if (autoPos != std::string::npos) {
        sourceText.replace(autoPos, 4, "int");
      }

      // Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), sourceText);
    }
}
  
void MutatorFrontendAction_323::MutatorASTConsumer_323::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation.
    auto matcher = conceptSpecializationExpr(hasDescendant(declRefExpr(to(varDecl(hasType(autoType())))))).bind("ConceptExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}