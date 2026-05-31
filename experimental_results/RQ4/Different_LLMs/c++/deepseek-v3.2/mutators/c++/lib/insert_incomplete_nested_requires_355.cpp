//source file
#include "../include/Insert_Incomplete_Nested_Requires_355.h"
#include "clang/AST/Expr.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

// ========================================================================================================
#define MUT355_OUTPUT 1

void MutatorFrontendAction_355::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *RE = Result.Nodes.getNodeAs<clang::Expr>("RequiresExpr")) {
      //Filter nodes in header files
      if (!RE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     RE->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto requiresRange = RE->getSourceRange();
      auto requiresText = stringutils::rangetoStr(*(Result.SourceManager), requiresRange);
      //Find the body of the requires expression
      auto bodyRange = RE->getSourceRange();
      auto bodyText = stringutils::rangetoStr(*(Result.SourceManager), bodyRange);
      //Perform mutation on the source code text by applying string replacement
      //Insert "requires ( ;" at the beginning of the body
      std::string mutatedBody = "requires ( ;\n" + bodyText;
      std::string mutatedRequires = requiresText;
      mutatedRequires.replace(mutatedRequires.find(bodyText), bodyText.length(), mutatedBody);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(requiresRange), mutatedRequires);
    }
}
  
void MutatorFrontendAction_355::MutatorASTConsumer_355::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = clang::ast_matchers::expr().bind("RequiresExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}