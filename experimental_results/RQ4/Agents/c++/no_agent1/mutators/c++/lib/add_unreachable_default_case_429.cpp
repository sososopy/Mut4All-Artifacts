//source file
#include "../include/add_unreachable_default_case_429.h"

// ========================================================================================================
#define MUT429_OUTPUT 1

void MutatorFrontendAction_429::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *SS = Result.Nodes.getNodeAs<clang::SwitchStmt>("SwitchStmt")) {
      //Filter nodes in header files
      if (!SS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     SS->getSwitchLoc()))
        return;

      //Get the source code text of target node
      auto switchSource = stringutils::rangetoStr(*(Result.SourceManager), SS->getSourceRange());

      //Perform mutation on the source code text by applying string replacement
      std::string defaultCase = "\n/*mut429*/default: __builtin_unreachable();\n";
      if (switchSource.find("default:") == std::string::npos) {
        switchSource.insert(switchSource.rfind('}'), defaultCase);
      }

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(SS->getSourceRange()), switchSource);
    }
}
  
void MutatorFrontendAction_429::MutatorASTConsumer_429::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    StatementMatcher matcher = switchStmt().bind("SwitchStmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}