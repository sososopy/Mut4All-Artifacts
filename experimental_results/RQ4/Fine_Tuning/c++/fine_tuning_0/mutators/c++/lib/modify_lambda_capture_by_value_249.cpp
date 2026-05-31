//source file
#include "../include/Modify_Lambda_Capture_By_Value_249.h"

// ========================================================================================================
#define MUT249_OUTPUT 1

void MutatorFrontendAction_249::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambda")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto capture = MT->capture_begin();
      for (; capture != MT->capture_end(); ++capture) {
        if (capture->getCaptureKind() == LCK_ByRef) {
          //Perform mutation on the source code text by applying string replacement
          auto capture_str = stringutils::rangetoStr(
              *(Result.SourceManager), capture->getLocation());
          capture_str.erase(capture_str.begin());
          llvm::outs() << capture_str << '\n';
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(capture->getLocation(), capture_str);
        }
      }
    }
}
  
void MutatorFrontendAction_249::MutatorASTConsumer_249::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = lambdaExpr().bind("Lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}