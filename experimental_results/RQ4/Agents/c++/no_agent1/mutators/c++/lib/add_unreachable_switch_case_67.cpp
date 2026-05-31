//source file
#include "../include/add_unreachable_switch_case_67.h"

// ========================================================================================================
#define MUT67_OUTPUT 1

void MutatorFrontendAction_67::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *SW = Result.Nodes.getNodeAs<clang::SwitchStmt>("SwitchStmt")) {
      //Filter nodes in header files
      if (!SW || !Result.Context->getSourceManager().isWrittenInMainFile(
                     SW->getSwitchLoc()))
        return;
      
      //Get the source code text of target node
      auto switchBody = stringutils::rangetoStr(*(Result.SourceManager),
                                                SW->getBody()->getSourceRange());
      
      //Perform mutation on the source code text by applying string replacement
      std::string unreachableCase = "\n/*mut67*/case 999999: break;";
      if (switchBody.rfind('}') != std::string::npos)
        switchBody.insert(switchBody.rfind('}'), unreachableCase);
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(SW->getBody()->getSourceRange()), switchBody);
    }
}
  
void MutatorFrontendAction_67::MutatorASTConsumer_67::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation
    StatementMatcher matcher = switchStmt().bind("SwitchStmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}