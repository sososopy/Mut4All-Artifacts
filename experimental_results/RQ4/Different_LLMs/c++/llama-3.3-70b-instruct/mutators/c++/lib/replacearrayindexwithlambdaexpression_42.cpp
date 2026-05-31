//source file
#include "../include/ReplaceArrayIndexWithLambdaExpression_42.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTContext.h"
#include "clang/Basic/SourceManager.h"

// ========================================================================================================
#define MUT42_OUTPUT 1

void MutatorFrontendAction_42::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *AE = Result.Nodes.getNodeAs<clang::ArraySubscriptExpr>("ArrayExpr")) {
      //Filter nodes in header files
      if (!AE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     AE->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto index = AE->getIdx();
      auto indexText = clang::Lexer::getSourceText(
          CharSourceRange::getTokenRange(index->getSourceRange()), 
          *Result.SourceManager, 
          Result.Context->getLangOpts());
      //Perform mutation on the source code text by applying string replacement
      auto lambdaText = "[] { return " + indexText + "; }()";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(AE->getSourceRange()), llvm::StringRef(lambdaText.str()));
    }
}
  
void MutatorFrontendAction_42::MutatorASTConsumer_42::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = arraySubscriptExpr().bind("ArrayExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}