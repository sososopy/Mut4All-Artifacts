//source file
#include "../include/Capture_This_And_Member_497.h"

// ========================================================================================================
#define MUT497_OUTPUT 1

void MutatorFrontendAction_497::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *Lambda = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambdaExpr")) {
      //Filter nodes in header files
      if (!Lambda || !Result.Context->getSourceManager().isWrittenInMainFile(
                     Lambda->getBeginLoc()))
        return;

      const CXXRecordDecl *ParentClass = Lambda->getLambdaClass();
      if (!ParentClass)
        return;

      // Find a member variable to capture by value
      for (const auto *Field : ParentClass->fields()) {
          if (Field->getType()->isIntegerType()) {
              memberField = Field;
              break;
          }
      }

      if (!memberField)
        return;

      //Get the source code text of target node
      auto lambdaSource = stringutils::rangetoStr(*(Result.SourceManager),
                                                  Lambda->getSourceRange());
      std::string memberName = memberField->getNameAsString();

      //Perform mutation on the source code text by applying string replacement
      std::string captureClause = "[this, " + memberName + "]";
      size_t captureStart = lambdaSource.find('[');
      size_t captureEnd = lambdaSource.find(']', captureStart);

      if (captureStart != std::string::npos && captureEnd != std::string::npos) {
          lambdaSource.replace(captureStart, captureEnd - captureStart + 1, captureClause);
      }

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(Lambda->getSourceRange()), lambdaSource);
    }
}
  
void MutatorFrontendAction_497::MutatorASTConsumer_497::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = lambdaExpr(hasAncestor(cxxRecordDecl())).bind("lambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}