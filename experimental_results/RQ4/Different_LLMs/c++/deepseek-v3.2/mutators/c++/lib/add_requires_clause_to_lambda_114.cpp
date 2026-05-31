//source file
#include "../include/Mutator_Add_Requires_Clause_To_Lambda_114.h"

// ========================================================================================================
#define MUT114_OUTPUT 1

void MutatorFrontendAction_114::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambdaExpr")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto lambdaDecl = MT->getLambdaClass();
      if (!lambdaDecl) return;
      
      auto callOperator = lambdaDecl->getLambdaCallOperator();
      if (!callOperator) return;
      
      // Check if lambda has parameters
      if (callOperator->param_empty()) return;
      
      // Get first parameter
      auto firstParam = *callOperator->param_begin();
      if (!firstParam) return;
      
      // Get source range for the lambda expression
      SourceRange lambdaRange = MT->getSourceRange();
      if (lambdaRange.isInvalid()) return;
      
      SourceManager &SM = *Result.SourceManager;
      LangOptions LangOpts = Result.Context->getLangOpts();
      
      // Get the full lambda text
      std::string lambdaText = stringutils::rangetoStr(SM, lambdaRange);
      
      // Find the position after parameters and before body
      SourceLocation bodyStart = MT->getBody()->getBeginLoc();
      SourceLocation lambdaStart = MT->getBeginLoc();
      
      // Get text from lambda start to body start
      std::string prefix = stringutils::rangetoStr(SM, SourceRange(lambdaStart, bodyStart.getLocWithOffset(-1)));
      
      // Check if lambda already has a requires clause
      bool hasRequires = false;
      if (callOperator->getTrailingRequiresClause()) {
        hasRequires = true;
      }
      
      // Get parameter name
      std::string paramName = firstParam->getNameAsString();
      if (paramName.empty()) {
        // Generate a placeholder if parameter has no name
        paramName = "x";
      }
      
      // Perform mutation on the source code text by applying string replacement
      std::string mutatedPrefix;
      if (hasRequires) {
        // If lambda already has requires clause, modify it to include requires requires
        // Find the requires clause in the prefix
        size_t requiresPos = prefix.find("requires");
        if (requiresPos != std::string::npos) {
          // Insert requires requires { param; } after the existing requires
          std::string newRequires = "requires requires { " + paramName + "; } ";
          mutatedPrefix = prefix.substr(0, requiresPos) + newRequires + prefix.substr(requiresPos);
        } else {
          // Fallback: append new requires clause
          mutatedPrefix = prefix + " requires requires { " + paramName + "; } ";
        }
      } else {
        // Add new requires clause
        mutatedPrefix = prefix + " requires requires { " + paramName + "; } ";
      }
      
      // Get the body text
      std::string bodyText = stringutils::rangetoStr(SM, MT->getBody()->getSourceRange());
      
      // Construct the mutated lambda
      std::string mutatedLambda = mutatedPrefix + bodyText;
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(lambdaRange), mutatedLambda);
    }
}
  
void MutatorFrontendAction_114::MutatorASTConsumer_114::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = lambdaExpr().bind("lambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}