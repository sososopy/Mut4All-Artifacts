//source file
#include "../include/ReplaceEnumWithIncompleteType_180.h"

// ========================================================================================================
#define MUT180_OUTPUT 1

void MutatorFrontendAction_180::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *ET = Result.Nodes.getNodeAs<clang::EnumDecl>("EnumDecl")) {
      //Filter nodes in header files
      if (!ET || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ET->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   ET->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::string enumName = ET->getNameAsString();
      std::string forwardDeclaration = "enum " + enumName + ";";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(ET->getSourceRange()), forwardDeclaration);
    }
}
  
void MutatorFrontendAction_180::MutatorASTConsumer_180::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = enumDecl().bind("EnumDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}