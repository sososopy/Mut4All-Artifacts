//source file
#include "../include/ArraySizeMutator_350.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include <string>

// ========================================================================================================
#define MUT350_OUTPUT 1

void MutatorFrontendAction_350::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *AD = Result.Nodes.getNodeAs<clang::ArraySubscriptExpr>("ArrayDecl")) {
      //Filter nodes in header files
      if (!AD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     AD->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto declaration = clang::Lexer::getSourceText(
          clang::CharSourceRange::getTokenRange(AD->getBeginLoc(), AD->getEndLoc()),
          *Result.SourceManager, Result.Context->getLangOpts());
      std::string declStr = declaration.str();
      //Perform mutation on the source code text by applying string replacement
      size_t pos = declStr.find('[');
      size_t endPos = declStr.find(']');
      if (pos != std::string::npos && endPos != std::string::npos) {
        std::string sizeStr = declStr.substr(pos + 1, endPos - pos - 1);
        int size = atoi(sizeStr.c_str());
        std::string mutatedSizeStr = std::to_string(-size);
        declStr.replace(pos + 1, endPos - pos - 1, mutatedSizeStr);
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(clang::CharSourceRange::getTokenRange(AD->getBeginLoc(), AD->getEndLoc()), declStr);
    }
}

void MutatorFrontendAction_350::MutatorASTConsumer_350::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = arraySubscriptExpr().bind("ArrayDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}