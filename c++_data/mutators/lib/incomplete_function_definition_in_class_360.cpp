//source file
#include "../include/incomplete_function_definition_in_class_360.h"

// ========================================================================================================
#define MUT360_OUTPUT 1

void MutatorFrontendAction_360::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CRD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("ClassOrStruct")) {
      //Filter nodes in header files
      if (!CRD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CRD->getLocation()))
        return;
      if (!CRD->isCompleteDefinition())
        return;

      //Get the source code text of target node
      std::string declaration = "void incompleteFunction(int y);";
      //Perform mutation on the source code text by applying string replacement
      std::string insertion = "\n/*mut360*/" + declaration + "\n";
      //Replace the original AST node with the mutated one
      Rewrite.InsertTextAfterToken(CRD->getEndLoc(), insertion);
    }
}
  
void MutatorFrontendAction_360::MutatorASTConsumer_360::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl(isDefinition()).bind("ClassOrStruct");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}