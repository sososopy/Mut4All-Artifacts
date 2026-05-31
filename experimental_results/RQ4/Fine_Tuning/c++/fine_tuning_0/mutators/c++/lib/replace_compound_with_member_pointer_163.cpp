//source file
#include "../include/Replace_Compound_With_Member_Pointer_163.h"

// ========================================================================================================
#define MUT163_OUTPUT 1

void MutatorFrontendAction_163::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (!FD->hasBody())
        return;
      auto body = FD->getBody();
      if (!(body->getStmtClassName() == string("CompoundStmt")))
        return;
      //Get the source code text of target node
      auto bodystr = stringutils::rangetoStr(*(Result.SourceManager),
                                             body->getSourceRange());
      llvm::outs() << bodystr << '\n';
      //Perform mutation on the source code text by applying string replacement
      string replacement = "{\n/*mut163*/auto mp;\nauto x = mp + 0;\n}";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(body->getBeginLoc(), bodystr.size(), replacement);
    }
}
  
void MutatorFrontendAction_163::MutatorASTConsumer_163::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl().bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}