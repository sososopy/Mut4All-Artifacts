//source file
#include "../include/Insert_Lambda_Expression_Within_Lambda_402.h"

// ========================================================================================================
#define MUT402_OUTPUT 1

void MutatorFrontendAction_402::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambda")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      auto capture = stringutils::rangetoStr(*(Result.SourceManager),
                                             MT->getCaptureDefaultLoc());
      llvm::outs() << capture << '\n';
      if (capture == "=")
        capture = "[=]";
      else
        capture = "[&]";
      string lambda_decl = "auto mut_402_lambda=" + capture + "{return 0;};";
      string lambda_call = "mut_402_lambda();";
      content.insert(content.find('{') + 1, lambda_decl);
      content.insert(content.rfind('}'), lambda_call);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_402::MutatorASTConsumer_402::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = lambdaExpr().bind("Lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}