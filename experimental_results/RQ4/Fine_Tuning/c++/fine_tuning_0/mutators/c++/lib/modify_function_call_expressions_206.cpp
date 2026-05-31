//source file
#include "../include/Modify_Function_Call_Expressions_206.h"

// ========================================================================================================
#define MUT206_OUTPUT 1

void MutatorFrontendAction_206::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CallExpr>("Functions")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          MT->getSourceRange());
      auto args = MT->getArgs();
      auto num_args = MT->getNumArgs();
      int choice = getrandom::getRandomIndex(1);
      if (choice) {
        int num_add = getrandom::getRandomIndex(2);
        for (int i = 0; i < num_add; ++i) {
          int index = getrandom::getRandomIndex(num_args - 1);
          auto arg = stringutils::rangetoStr(*(Result.SourceManager),
                                             args[index]->getSourceRange());
          if (text.rfind(')') != string::npos)
            text.insert(text.rfind(')'), "," + arg);
        }
      } else {
        int num_del = getrandom::getRandomIndex(num_args - 1);
        for (int i = 0; i < num_del; ++i) {
          int index = getrandom::getRandomIndex(num_args - 1);
          auto arg = stringutils::rangetoStr(*(Result.SourceManager),
                                             args[index]->getSourceRange());
          auto pos = text.find(arg);
          text.erase(pos, arg.size());
        }
      }
      //Perform mutation on the source code text by applying string replacement
      llvm::outs() << text << '\n';
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), text);
    }
}
  
void MutatorFrontendAction_206::MutatorASTConsumer_206::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = callExpr().bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}