//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Fold_Expression_In_Requires_115
 */ 
class MutatorFrontendAction_115 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(115)

private:
    class MutatorASTConsumer_115 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_115(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_fold_expression_in_requires_115.h"

// ========================================================================================================
#define MUT115_OUTPUT 1

void MutatorFrontendAction_115::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *Expr = Result.Nodes.getNodeAs<clang::BinaryOperator>("FoldExpr")) {
      if (!Expr || !Result.Context->getSourceManager().isWrittenInMainFile(
                     Expr->getBeginLoc()))
        return;

      auto SourceText = stringutils::rangetoStr(*(Result.SourceManager),
                                                Expr->getSourceRange());

      std::string NewConstraint = "(requires (T t) { typename std::conditional<sizeof...(Ts) != 0, int, void>::type{}; }) && ";

      size_t InsertPos = SourceText.find("&&");
      if (InsertPos != std::string::npos) {
        SourceText.insert(InsertPos, NewConstraint);
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(Expr->getSourceRange()), SourceText);
      }
    }
}
  
void MutatorFrontendAction_115::MutatorASTConsumer_115::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = binaryOperator(hasOperatorName("&&"), 
                                                hasLHS(expr(hasType(cxxBoolType()))), 
                                                hasRHS(expr(hasType(cxxBoolType())))).bind("FoldExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}