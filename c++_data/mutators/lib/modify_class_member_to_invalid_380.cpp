//source file
#include "../include/Modify_Class_Member_To_Invalid_380.h"

// ========================================================================================================
#define MUT380_OUTPUT 1

void MutatorFrontendAction_380::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FieldDecl>("Field")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Get the source code text of target node
      auto fieldName = FD->getNameAsString();
      if (fieldName.empty())
        return;
      
      //Perform mutation on the source code text by applying string replacement
      std::string invalidName = "1" + fieldName; // Invalid identifier starting with a number
      Rewrite.ReplaceText(FD->getLocation(), fieldName.length(), invalidName);
    }
}
  
void MutatorFrontendAction_380::MutatorASTConsumer_380::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = fieldDecl().bind("Field");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}