//source file
#include "../include/Mutator_570.h"

// ========================================================================================================
#define MUT570_OUTPUT 1

void MutatorFrontendAction_570::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      
      //Get the source code text of target node
      auto params = FD->parameters();
      if (params.empty()) return;

      // Select a parameter to duplicate
      auto *paramToDuplicate = params[0];  // For simplicity, choose the first parameter
      std::string paramText = paramToDuplicate->getType().getAsString() + " " + paramToDuplicate->getNameAsString();

      //Perform mutation on the source code text by applying string replacement
      SourceLocation insertLoc = paramToDuplicate->getSourceRange().getEnd();
      Rewrite.InsertTextAfterToken(insertLoc, ", " + paramText);
    }
}
  
void MutatorFrontendAction_570::MutatorASTConsumer_570::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl().bind("FunctionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}