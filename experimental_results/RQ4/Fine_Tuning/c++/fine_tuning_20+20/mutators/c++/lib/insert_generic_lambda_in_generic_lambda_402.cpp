//source file
#include "../include/Insert_Generic_Lambda_In_Generic_Lambda_402.h"

// ========================================================================================================
#define MUT402_OUTPUT 1

void MutatorFrontendAction_402::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambda")) {
      //Filter nodes in header files
      if (!LE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LE->getBeginLoc()))
        return;
      if (LE->isGenericLambda())
        return;
      //Get the source code text of target node
      auto lambda_text = stringutils::rangetoStr(*(Result.SourceManager),
                                                 LE->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      if (lambda_text.find('[') != string::npos)
        lambda_text.insert(lambda_text.find('['), "/*mut402*/");
      if (lambda_text.find(']') != string::npos)
        lambda_text.insert(lambda_text.find(']') + 1, "(auto a)");
      if (lambda_text.find('{') != string::npos)
        lambda_text.insert(lambda_text.find('{') + 1, "\nauto l2=[](auto b){};");
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(LE->getSourceRange()), lambda_text);
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