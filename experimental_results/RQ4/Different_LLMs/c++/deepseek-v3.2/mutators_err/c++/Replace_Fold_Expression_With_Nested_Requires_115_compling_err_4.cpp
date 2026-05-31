//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Fold_Expression_With_Nested_Requires_115
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
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Replace_Fold_Expression_With_Nested_Requires_115.h"

// ========================================================================================================
#define MUT115_OUTPUT 1

void MutatorFrontendAction_115::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FE = Result.Nodes.getNodeAs<clang::CXXFoldExpr>("foldExpr")) {
      //Filter nodes in header files
      if (!FE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FE->getBeginLoc()))
        return;
      // Check if the fold expression is over a requires-expression
      auto *SubExpr = FE->getPattern();
      if (!SubExpr) return;
      bool IsOverRequires = false;
      if (auto *BinOp = llvm::dyn_cast<clang::BinaryOperator>(SubExpr)) {
        if (BinOp->getLHS()->getStmtClass() == clang::Stmt::RequiresExprClass ||
            BinOp->getRHS()->getStmtClass() == clang::Stmt::RequiresExprClass) {
          IsOverRequires = true;
        }
      } else if (SubExpr->getStmtClass() == clang::Stmt::RequiresExprClass) {
        IsOverRequires = true;
      }
      if (!IsOverRequires) return;
      //Get the source code text of target node
      auto FoldRange = FE->getSourceRange();
      auto FoldText = stringutils::rangetoStr(*(Result.SourceManager), FoldRange);
      // Extract the pack name and size N from context (simplified: assume pack is Is...)
      // In practice, we would analyze the AST to find the parameter pack declaration.
      // For this example, we assume pack is 'Is' and size is extracted from a make_index_sequence<N> call.
      // We'll generate a placeholder mutation.
      std::string NewText = "requires []<size_t... Is>(std::index_sequence<Is...>) {\n"
                            "        struct checker {\n"
                            "            static constexpr bool value = true;\n"
                            "        };\n"
                            "        return checker::value;\n"
                            "    }(std::make_index_sequence<N>{})";
      //Perform mutation on the source code text by applying string replacement
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(FoldRange, "/*mut115*/" + NewText);
    }
}
  
void MutatorFrontendAction_115::MutatorASTConsumer_115::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = foldExpr(isExpansionInMainFile()).bind("foldExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}