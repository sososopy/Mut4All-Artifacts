//source file
#include "../include/modify_builtin_fpclassify_argument_11.h"

// ========================================================================================================
#define MUT11_OUTPUT 1

void MutatorFrontendAction_11::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("builtinCall")) {
      //Filter nodes in header files
      if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CE->getBeginLoc()))
        return;

      //Get the source code text of target node
      auto args = CE->getArgs();
      for (unsigned i = 0; i < CE->getNumArgs(); ++i) {
        if (auto *IL = llvm::dyn_cast<clang::IntegerLiteral>(args[i])) {
          //Perform mutation on the source code text by applying string replacement
          if (IL->getValue() == 0 || IL->getValue() == 1 || IL->getValue() == 4 || IL->getValue() == 3 || IL->getValue() == 2) {
            auto loc = IL->getBeginLoc();
            Rewrite.ReplaceText(loc, "invalid");
            break;
          }
        }
      }
    }
}
  
void MutatorFrontendAction_11::MutatorASTConsumer_11::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = callExpr(callee(functionDecl(hasName("__builtin_fpclassify")))).bind("builtinCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}