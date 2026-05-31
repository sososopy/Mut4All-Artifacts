//source file
#include "../include/ReplaceLambdaWithFunctionPointer_78.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTContext.h"
#include "clang/Rewrite/Core/Rewriter.h"

// ========================================================================================================
#define MUT78_OUTPUT 1

void MutatorFrontendAction_78::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *Call = Result.Nodes.getNodeAs<clang::CallExpr>("CallExpr")) {
      //Filter nodes in header files
      if (!Call || !Result.Context->getSourceManager().isWrittenInMainFile(
                     Call->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto args = Call->arguments();
      for (auto arg : args) {
        if (auto *Lambda = dyn_cast<clang::LambdaExpr>(arg)) {
          //Perform mutation on the source code text by applying string replacement
          std::string funcName = "lambda_func_" + std::to_string(getrandom::getRandomIndex(1000));
          std::string funcDecl = "auto " + funcName + " = " + stringutils::rangetoStr(*(Result.SourceManager), Lambda->getSourceRange()) + ";";
          std::string funcPtrDecl = funcName;
          //Replace the original AST node with the mutated one
          Rewrite.InsertTextBefore(Call->getBeginLoc(), funcDecl + "\n");
          Rewrite.ReplaceText(Lambda->getSourceRange(), funcPtrDecl);
        }
      }
    }
}
  
void MutatorFrontendAction_78::MutatorASTConsumer_78::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = clang::ast_matchers::callExpr(hasArgument(0, clang::ast_matchers::lambdaExpr())).bind("CallExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}