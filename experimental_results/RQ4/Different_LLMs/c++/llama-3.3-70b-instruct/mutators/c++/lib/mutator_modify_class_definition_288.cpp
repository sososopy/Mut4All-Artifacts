//source file
#include "../include/Modify_Class_Definition_288.h"

// ========================================================================================================
#define MUT288_OUTPUT 1

void MutatorFrontendAction_288::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      // Change access specifier
      if (declaration.find("public") != std::string::npos) {
        declaration.replace(declaration.find("public"), 6, "private");
      } else if (declaration.find("private") != std::string::npos) {
        declaration.replace(declaration.find("private"), 7, "protected");
      } else if (declaration.find("protected") != std::string::npos) {
        declaration.replace(declaration.find("protected"), 9, "public");
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_288::MutatorASTConsumer_288::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}