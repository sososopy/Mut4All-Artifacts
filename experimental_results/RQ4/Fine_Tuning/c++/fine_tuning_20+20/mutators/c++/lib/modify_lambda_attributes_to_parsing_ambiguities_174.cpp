//source file
#include "../include/Modify_Lambda_Attributes_To_Parsing_Ambiguities_174.h"

// ========================================================================================================
#define MUT174_OUTPUT 1

void MutatorFrontendAction_174::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambda")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto lambda = stringutils::rangetoStr(*(Result.SourceManager),
                                            MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      llvm::outs() << lambda << '\n';
      if (lambda.find("return") != string::npos) {
        lambda.erase(lambda.find("return"), 6);
        lambda.insert(lambda.rfind('}'), ";");
        lambda.insert(lambda.rfind('}'), "({");
        lambda.insert(lambda.find("]") + 1, "(void)");
        lambda.insert(lambda.find("]") + 1, "__attribute__((b)");
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), lambda);
    }
}
  
void MutatorFrontendAction_174::MutatorASTConsumer_174::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = lambdaExpr().bind("Lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}