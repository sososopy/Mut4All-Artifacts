//source file
#include "../include/Insert_Malformed_Asm_Statement_272.h"

// ========================================================================================================
#define MUT272_OUTPUT 1

void MutatorFrontendAction_272::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (!FD->hasBody())
        return;
      //Get the source code text of target node
      Stmt *Body = FD->getBody();
      if (!Body)
        return;
      SourceLocation EndLoc = Body->getEndLoc();
      //Perform mutation on the source code text by applying string replacement
      std::string MalformedAsm = "asm(\"nop\"";
      //Replace the original AST node with the mutated one
      Rewrite.InsertTextBefore(EndLoc, MalformedAsm);
    }
}
  
void MutatorFrontendAction_272::MutatorASTConsumer_272::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl().bind("FunctionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}