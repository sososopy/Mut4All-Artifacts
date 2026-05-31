//source file
#include "../include/Constructor_With_Explicit_Object_Parameter_353.h"

// ========================================================================================================
#define MUT353_OUTPUT 1

void MutatorFrontendAction_353::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("MethodWithExplicitThis")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      if (MT->getDeclName().isIdentifier() && MT->getName() == MT->getParent()->getName())
        return;
      
      auto methodText = stringutils::rangetoStr(*(Result.SourceManager),
                                                MT->getSourceRange());
      //(optional)Record the node information to be used in the mutation process
      auto className = MT->getParent()->getNameAsString();
      //Perform mutation on the source code text by applying string replacement
      size_t pos = methodText.find(MT->getNameAsString());
      if (pos != std::string::npos) {
          methodText.replace(pos, MT->getNameAsString().length(), className);
      }
      //Remove return type if present
      size_t typePos = methodText.find_first_not_of(" ");
      if (typePos != std::string::npos) {
          size_t namePos = methodText.find(className);
          if (namePos > typePos) {
              methodText.erase(typePos, namePos - typePos);
          }
      }
      methodText = "/*mut353*/" + methodText;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), methodText);
    }
}
  
void MutatorFrontendAction_353::MutatorASTConsumer_353::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxMethodDecl(unless(cxxConstructorDecl())).bind("MethodWithExplicitThis");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}