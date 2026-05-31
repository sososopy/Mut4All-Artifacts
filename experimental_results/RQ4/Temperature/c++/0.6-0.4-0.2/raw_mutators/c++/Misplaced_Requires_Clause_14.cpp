//header file
#pragma once
#include "Mutator_base.h"

/**
 * misplaced_requires_clause_14
 */ 
class MutatorFrontendAction_14 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(14)

private:
    class MutatorASTConsumer_14 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_14(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/misplaced_requires_clause_14.h"

// ========================================================================================================
#define MUT14_OUTPUT 1

void MutatorFrontendAction_14::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CD = Result.Nodes.getNodeAs<clang::ConceptDecl>("ConceptDecl")) {
      if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(CD->getLocation()))
        return;

      auto conceptSrc = stringutils::rangetoStr(*(Result.SourceManager), CD->getSourceRange());
      size_t requiresPos = conceptSrc.find("requires");
      if (requiresPos != std::string::npos) {
        conceptSrc.insert(requiresPos + 8, " requires");
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(CD->getSourceRange()), conceptSrc);
      }
    }
}
  
void MutatorFrontendAction_14::MutatorASTConsumer_14::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = conceptDecl().bind("ConceptDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}