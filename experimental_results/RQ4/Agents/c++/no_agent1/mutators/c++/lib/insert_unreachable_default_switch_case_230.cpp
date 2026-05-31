//source file
#include "../include/insert_unreachable_default_switch_case_230.h"

// ========================================================================================================
#define MUT230_OUTPUT 1

void MutatorFrontendAction_230::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *ST = Result.Nodes.getNodeAs<clang::SwitchStmt>("Switch")) {
      //Filter nodes in header files
      if (!ST || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ST->getSwitchLoc()))
        return;
      
      //Get the source code text of target node
      auto switchBody = stringutils::rangetoStr(*(Result.SourceManager), ST->getBody()->getSourceRange());

      //Perform mutation on the source code text by applying string replacement
      std::string unreachableDefault = "default: __builtin_unreachable();";
      if (switchBody.find("default:") == std::string::npos) {
        switchBody.insert(switchBody.rfind('}'), "\n/*mut230*/" + unreachableDefault + "\n");
      }

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(ST->getBody()->getSourceRange()), switchBody);
    }
}

void MutatorFrontendAction_230::MutatorASTConsumer_230::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    StatementMatcher matcher = switchStmt().bind("Switch");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}