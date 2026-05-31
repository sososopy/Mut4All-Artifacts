//source file
#include "../include/Add_Variadic_Template_Instantiation_Within_Lambda_183.h"

// ========================================================================================================
#define MUT183_OUTPUT 1

void MutatorFrontendAction_183::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *LT = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambdas")) {
      //Filter nodes in header files
      if (!LT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             LT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      content.insert(content.find("{") + 1, "\n/*mut183*/func(args...);\n");
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(LT->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_183::MutatorASTConsumer_183::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher =
        functionTemplateDecl(hasName("func")).bind("VariadicTemplates");
    auto lambda_matcher = lambdaExpr().bind("Lambdas");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(lambda_matcher, &callback);
    matchFinder.matchAST(Context);
}