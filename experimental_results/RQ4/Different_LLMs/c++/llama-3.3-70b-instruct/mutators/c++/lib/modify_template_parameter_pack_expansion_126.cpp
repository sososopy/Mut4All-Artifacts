//source file
#include "../include/Modify_Template_Parameter_Pack_Expansion_126.h"

// ========================================================================================================
#define MUT126_OUTPUT 1

void MutatorFrontendAction_126::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::LambdaExpr>(("Lambda"))) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      // Remove or add a parameter from the pack
      int choice = getrandom::getRandomIndex(1);
      if (choice) {
        // Remove a parameter
        size_t pos = declaration.find(")");
        if (pos != string::npos) {
          size_t commaPos = declaration.rfind(",", pos);
          if (commaPos != string::npos) {
            declaration.erase(commaPos, 1);
          }
        }
      } else {
        // Add a parameter
        size_t pos = declaration.find(")");
        if (pos != string::npos) {
          declaration.insert(pos, ", int");
        }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_126::MutatorASTConsumer_126::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = lambdaExpr().bind("Lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}