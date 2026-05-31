//source file
#include "../include/Insert_Incomplete_Destructor_336.h"

// ========================================================================================================
#define MUT336_OUTPUT 1

void MutatorFrontendAction_336::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      //Filter nodes in header files
      if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CL->getLocation()))
        return;
      if ((!CL->isStruct() && !CL->isClass()) || CL->isLambda())
        return;
      if (!CL->isCompleteDefinition())
        return;
      
      //Get the source code text of target node
      std::string className = CL->getNameAsString();
      std::string incorrectDestructor = "~" + className + "Incomplete();\n";
      incorrectDestructor = "/*mut336*/" + incorrectDestructor;
      
      //Perform mutation on the source code text by applying string replacement
      Rewrite.InsertTextAfterToken(CL->getEndLoc(), incorrectDestructor);
    }
}
  
void MutatorFrontendAction_336::MutatorASTConsumer_336::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}