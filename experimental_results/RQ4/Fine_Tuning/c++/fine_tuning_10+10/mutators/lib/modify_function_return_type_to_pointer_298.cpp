//source file
#include "../include/Modify_Function_Return_Type_To_Pointer_298.h"

// ========================================================================================================
#define MUT298_OUTPUT 1

void MutatorFrontendAction_298::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      //Get the source code text of target node
      auto returnType = FD->getReturnType();
      if (returnType->isPointerType())
        return; // Already a pointer type, skip

      //Perform mutation on the source code text by applying string replacement
      std::string newReturnType = returnType.getAsString() + "*";
      SourceLocation typeStartLoc = FD->getReturnTypeSourceRange().getBegin();
      Rewrite.ReplaceText(typeStartLoc, returnType.getAsString().length(), newReturnType);
    }
}
  
void MutatorFrontendAction_298::MutatorASTConsumer_298::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl().bind("FunctionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}