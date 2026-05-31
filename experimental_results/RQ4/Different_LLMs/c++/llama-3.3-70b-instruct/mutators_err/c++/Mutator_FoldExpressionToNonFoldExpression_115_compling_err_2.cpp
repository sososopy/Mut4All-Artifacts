//header file
#pragma once
#include "Mutator_base.h"
#include "clang/AST/StmtNodes.h" // Include the necessary header for RequiresExpr

/**
 * FoldExpressionToNonFoldExpression_115
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
        virtual void run(const MatchFinder::MatchResult &Result) override; // Add override keyword
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Mutator_FoldExpressionToNonFoldExpression_115.h"
#include "clang/ASTMatchers/ASTMatchers.h" // Include the necessary header for ExprMatcher

// ========================================================================================================
#define MUT115_OUTPUT 1

void MutatorFrontendAction_115::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *RE = Result.Nodes.getNodeAs<clang::RequiresExpr>(("RequiresExpr"))) {
      //Filter nodes in header files
      if (!RE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     RE->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto requires_expr = stringutils::rangetoStr(*(Result.SourceManager), RE->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      // Replace the fold expression with a non-fold expression
      std::string mutated_requires_expr = requires_expr;
      size_t pos = mutated_requires_expr.find("&& ...");
      if (pos != std::string::npos) {
        mutated_requires_expr.replace(pos, 6, ")");
      }
      pos = mutated_requires_expr.find("(");
      if (pos != std::string::npos) {
        mutated_requires_expr.insert(pos + 1, "requires (T t) {");
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(RE->getSourceRange()), mutated_requires_expr);
    }
}
  
void MutatorFrontendAction_115::MutatorASTConsumer_115::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    ExprMatcher matcher = requiresExpr().bind("RequiresExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}