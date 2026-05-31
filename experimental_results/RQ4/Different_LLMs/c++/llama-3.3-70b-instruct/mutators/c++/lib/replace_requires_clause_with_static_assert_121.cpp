//source file
#include "../include/Replace_Requires_Clause_With_Static_Assert_121.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTContext.h"
#include "clang/Basic/SourceManager.h"

// ========================================================================================================
#define MUT121_OUTPUT 1

void MutatorFrontendAction_121::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::RequiresExpr>("RequiresExpr")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto requiresClause = clang::Lexer::getSourceText(
          clang::CharSourceRange::getTokenRange(MT->getSourceRange()), 
          *Result.SourceManager, 
          Result.Context->getLangOpts());
      //Perform mutation on the source code text by applying string replacement
      std::string staticAssert = "static_assert(" + requiresClause + ", \"requires clause failed\");";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(MT->getSourceRange(), staticAssert);
    }
}

void MutatorFrontendAction_121::MutatorASTConsumer_121::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = clang::ast_matchers::requiresExpr().bind("RequiresExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}