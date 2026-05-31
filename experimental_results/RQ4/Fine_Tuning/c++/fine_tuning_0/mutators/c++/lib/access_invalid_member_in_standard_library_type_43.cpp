//source file
#include "../include/Access_Invalid_Member_In_Standard_Library_Type_43.h"

// ========================================================================================================
#define MUT43_OUTPUT 1

void MutatorFrontendAction_43::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("StandardLibraryType")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto name = MT->getNameAsString();
      //Perform mutation on the source code text by applying string replacement
      name += ".d";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(MT->getLocation(), name);
    }
}
  
void MutatorFrontendAction_43::MutatorASTConsumer_43::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl(isExpansionInMainFile(), hasAnyName("vector", "string", "map", "set", "unordered_map", "unordered_set")).bind("StandardLibraryType");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}