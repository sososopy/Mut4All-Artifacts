//source file
#include "../include/modify_lambda_return_type_with_template_parameters_183.h"

// ========================================================================================================
#define MUT183_OUTPUT 1

void MutatorFrontendAction_183::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *lambdaExpr = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambdaExpr")) {
      //Filter nodes in header files
      if (!lambdaExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                     lambdaExpr->getBeginLoc()))
        return;

      auto *templateFunc = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("templateFunc");
      if (!templateFunc)
        return;

      //Get the source code text of target node
      auto lambdaSourceRange = lambdaExpr->getSourceRange();
      std::string lambdaSource = stringutils::rangetoStr(*(Result.SourceManager), lambdaSourceRange);

      //Perform mutation on the source code text by applying string replacement
      // Check if the lambda has a specified return type
      if (lambdaExpr->hasExplicitResultType()) {
        // Introduce a new template parameter for the return type
        std::string newTemplateParam = "typename R";
        std::string newReturnType = "R";

        // Insert the new template parameter into the function template
        auto templateParams = templateFunc->getTemplateParameters();
        if (templateParams->size() > 0) {
          std::string templateText = stringutils::rangetoStr(*(Result.SourceManager), templateParams->getSourceRange());
          templateText.insert(templateText.size() - 1, ", " + newTemplateParam);
          Rewrite.ReplaceText(templateParams->getSourceRange(), templateText);
        }

        // Replace the lambda's return type with the new template parameter
        size_t arrowPos = lambdaSource.find("->");
        if (arrowPos != std::string::npos) {
          size_t endPos = lambdaSource.find("{", arrowPos);
          lambdaSource.replace(arrowPos + 2, endPos - arrowPos - 2, newReturnType);
        }

        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(lambdaSourceRange, lambdaSource);
      }
    }
}
  
void MutatorFrontendAction_183::MutatorASTConsumer_183::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto lambdaMatcher = lambdaExpr(hasParent(functionTemplateDecl().bind("templateFunc"))).bind("lambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(lambdaMatcher, &callback);
    matchFinder.matchAST(Context);
}