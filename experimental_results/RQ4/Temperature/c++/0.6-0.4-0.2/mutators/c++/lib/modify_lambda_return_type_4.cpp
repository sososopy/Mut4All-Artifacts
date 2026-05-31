//source file
#include "../include/modify_lambda_return_type_4.h"

// ========================================================================================================
#define MUT4_OUTPUT 1

void MutatorFrontendAction_4::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("LambdaExpr")) {
      //Filter nodes in header files
      if (!LE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LE->getBeginLoc()))
        return;

      const auto *LambdaType = LE->getCallOperator()->getType()->getAs<FunctionProtoType>();
      if (LambdaType && LambdaType->getReturnType()->isBuiltinType()) {
        //Get the source code text of target node
        auto lambdaSource = stringutils::rangetoStr(*(Result.SourceManager),
                                                    LE->getSourceRange());
        //Perform mutation on the source code text by applying string replacement
        auto returnTypeLoc = lambdaSource.find("->");
        if (returnTypeLoc != std::string::npos) {
          auto endOfReturnType = lambdaSource.find("{", returnTypeLoc);
          if (endOfReturnType != std::string::npos) {
            auto originalReturnType = lambdaSource.substr(returnTypeLoc + 2, endOfReturnType - returnTypeLoc - 2);
            std::string mutatedReturnType = originalReturnType + "(" + originalReturnType + ")";
            lambdaSource.replace(returnTypeLoc + 2, endOfReturnType - returnTypeLoc - 2, mutatedReturnType);
            //Replace the original AST node with the mutated one
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(LE->getSourceRange()), lambdaSource);
          }
        }
      }
    }
}
  
void MutatorFrontendAction_4::MutatorASTConsumer_4::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = lambdaExpr().bind("LambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}