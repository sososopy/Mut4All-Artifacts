//source file
#include "../include/ReplaceLambdaCapture_16.h"
#include "clang/AST/ASTContext.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Basic/SourceLocation.h"

// ========================================================================================================
#define MUT16_OUTPUT 1

void MutatorFrontendAction_16::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *LT = Result.Nodes.getNodeAs<clang::LambdaExpr>(("Lambda"))) {
      //Filter nodes in header files
      if (!LT || !Result.Context->getSourceManager().isInMainFile(LT->getBeginLoc()))
        return;
      //Get the source code text of target node
      llvm::StringRef declaration = clang::Lexer::getSourceText(
          clang::CharSourceRange::getTokenRange(LT->getSourceRange()),
          *Result.SourceManager, 
          Result.Context->getLangOpts());
      std::string declarationStr(declaration.begin(), declaration.end());
      //Perform mutation on the source code text by applying string replacement
      if (declarationStr.find("[") != std::string::npos && declarationStr.find("]") != std::string::npos) {
        size_t start = declarationStr.find("[");
        size_t end = declarationStr.find("]");
        std::string capture = declarationStr.substr(start + 1, end - start - 1);
        if (capture.find("=") != std::string::npos) {
          // Replace value capture with reference capture
          declarationStr.replace(start + 1, end - start - 1, "&" + capture.substr(0, capture.find("=")));
        } else if (capture.find("&") != std::string::npos) {
          // Replace reference capture with value capture
          declarationStr.replace(start + 1, end - start - 1, capture.substr(0, capture.find("&")) + "=" + capture.substr(capture.find("&") + 1));
        } else if (capture == "") {
          // Replace default capture with value capture
          declarationStr.replace(start + 1, end - start - 1, "=");
        }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(clang::CharSourceRange::getTokenRange(LT->getSourceRange()), declarationStr);
    }
}
  
void MutatorFrontendAction_16::MutatorASTConsumer_16::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = lambdaExpr().bind("Lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}