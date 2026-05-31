//header file
#pragma once
#include "Mutator_base.h"

/**
 * Mutate_Template_Requires_Clause_441
 */ 
class MutatorFrontendAction_441 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(441)

private:
    class MutatorASTConsumer_441 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_441(Rewriter &R) : TheRewriter(R) {}
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
        const clang::Expr *cur_expr;
    };
};

//source file
#include "../include/Mutate_Template_Requires_Clause_441.h"

// ========================================================================================================
#define MUT441_OUTPUT 1

void MutatorFrontendAction_441::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::ConceptSpecializationExpr>("ConceptExpr")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;

      cur_expr = MT;
    } else if (auto *RT = Result.Nodes.getNodeAs<clang::RequiresExpr>(
                   "RequiresExpr")) {
      if (!RT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     RT->getBeginLoc()))
        return;
      if (cur_expr == nullptr)
        return;
      auto expr_str = stringutils::rangetoStr(*(Result.SourceManager),
                                              cur_expr->getSourceRange());
      auto choice = getrandom::getRandomIndex(1);
      if (choice) {
        expr_str = expr_str + " && true";
      } else {
        expr_str = expr_str + " || false";
      }
      llvm::outs() << expr_str << '\n';
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(cur_expr->getSourceRange()), expr_str);
    }
}
  
void MutatorFrontendAction_441::MutatorASTConsumer_441::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = conceptSpecializationExpr().bind("ConceptExpr");
    auto requires_matcher = requiresExpr().bind("RequiresExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(requires_matcher, &callback);
    matchFinder.matchAST(Context);
}