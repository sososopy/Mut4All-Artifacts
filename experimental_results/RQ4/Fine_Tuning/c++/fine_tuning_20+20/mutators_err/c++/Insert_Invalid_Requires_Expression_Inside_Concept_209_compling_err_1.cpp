//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Invalid_Requires_Expression_Inside_Concept_209
 */ 
class MutatorFrontendAction_209 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(209)

private:
    class MutatorASTConsumer_209 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_209(Rewriter &R) : TheRewriter(R) {}
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
        const clang::ConceptDecl *cur_concept;
    };
};

//source file
#include "../include/insert_invalid_requires_expression_inside_concept_209.h"

// ========================================================================================================
#define MUT209_OUTPUT 1

void MutatorFrontendAction_209::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CD = Result.Nodes.getNodeAs<clang::ConceptDecl>("Concepts")) {
      if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CD->getLocation()))
        return;
      cur_concept = CD;
    } else if (auto *RE =
                   Result.Nodes.getNodeAs<clang::RequiresExpr>("Requires")) {
      if (!RE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     RE->getBeginLoc()))
        return;
      if (cur_concept == nullptr)
        return;
      if (cur_concept->getRequiresExprBody() == nullptr)
        return;
      if (cur_concept->getRequiresExprBody()->getBeginLoc() >
              RE->getBeginLoc() ||
          cur_concept->getRequiresExprBody()->getEndLoc() <
              RE->getEndLoc()) // Ensure that the requires expression is inside the concept definition
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             RE->getSourceRange());
      int dice = getrandom::getRandomIndex(2);
      if (dice == 0) {
        content = "requires " + content;
      } else if (dice == 1) {
        content = content + " requires";
      } else if (dice == 2) {
        content = content.insert(content.find('{') + 1, "requires");
      } else
        assert(false && "Mut209: Dice should have 3 possible values!");
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(RE->getSourceRange()),
                          content);
    }
}
  
void MutatorFrontendAction_209::MutatorASTConsumer_209::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = conceptDecl().bind("Concepts");
    auto requires_matcher = requiresExpr().bind("Requires");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(requires_matcher, &callback);
    matchFinder.matchAST(Context);
}