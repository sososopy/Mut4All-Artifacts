//source file
#include "../include/Modify_Function_Call_To_Nonexistent_Name_40.h"

// ========================================================================================================
#define MUT40_OUTPUT 1

void MutatorFrontendAction_40::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CallExpr>("FuncCall")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      if (auto *FD = MT->getDirectCallee()) {
        //Get the source code text of target node
        auto name = FD->getNameAsString();
        llvm::outs() << name;
        //Perform mutation on the source code text by applying string replacement
        if (name.length() > 1) {
          int index = getrandom::getRandomIndex(name.length() - 1);
          name.erase(index, 1);
        }
        name = "/*mut40*/" + name;
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(MT->getBeginLoc(), FD->getNameAsString().length(),
                            name);
      }
    }
}
  
void MutatorFrontendAction_40::MutatorASTConsumer_40::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = callExpr().bind("FuncCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}