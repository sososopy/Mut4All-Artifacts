//source file
#include "../include/MutatorFrontendAction_273.h"

// ========================================================================================================
#define MUT273_OUTPUT 1

void MutatorFrontendAction_273::Callback::run(const MatchFinder::MatchResult &Result) {
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
      //Find the last return statement or end of block
      SourceLocation InsertLoc;
      if (auto *CS = dyn_cast<CompoundStmt>(Body)) {
        const Stmt *LastStmt = nullptr;
        for (const Stmt *S : CS->body()) {
          if (isa<ReturnStmt>(S)) {
            InsertLoc = S->getBeginLoc();
            break;
          }
          LastStmt = S;
        }
        if (!InsertLoc.isValid()) {
          if (LastStmt)
            InsertLoc = LastStmt->getEndLoc();
          else
            InsertLoc = CS->getBeginLoc().getLocWithOffset(1);
        }
      } else {
        InsertLoc = Body->getBeginLoc().getLocWithOffset(1);
      }
      //Perform mutation on the source code text by applying string replacement
      std::string AsmStmt = "\nasm(\"\");\n";
      //Replace the original AST node with the mutated one
      Rewrite.InsertTextBefore(InsertLoc, AsmStmt);
    }
}
  
void MutatorFrontendAction_273::MutatorASTConsumer_273::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl().bind("FunctionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}