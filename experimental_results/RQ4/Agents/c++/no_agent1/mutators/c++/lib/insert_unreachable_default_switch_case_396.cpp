//source file
#include "../include/Insert_Unreachable_Default_Switch_Case_396.h"

// ========================================================================================================
#define MUT396_OUTPUT 1

void MutatorFrontendAction_396::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *ST = Result.Nodes.getNodeAs<clang::SwitchStmt>("SwitchStmt")) {
      //Filter nodes in header files
      if (!ST || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ST->getBeginLoc()))
        return;

      //Get the source code text of target node
      auto switchBody = stringutils::rangetoStr(*(Result.SourceManager),
                                                ST->getBody()->getSourceRange());
      
      //Perform mutation on the source code text by applying string replacement
      std::string unreachableDefault = "\n/*mut396*/default: __builtin_unreachable();\n";
      if (switchBody.find("default:") == std::string::npos) {
        switchBody.insert(switchBody.rfind('}'), unreachableDefault);
      }

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(ST->getBody()->getSourceRange()), switchBody);
    }
}
  
void MutatorFrontendAction_396::MutatorASTConsumer_396::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    StatementMatcher matcher = switchStmt().bind("SwitchStmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}