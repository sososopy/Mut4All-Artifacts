//source file
#include "../include/incorrect_template_argument_substitution_55.h"

// ========================================================================================================
#define MUT55_OUTPUT 1

void MutatorFrontendAction_55::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateDecl")) {
      //Filter nodes in header files
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;

      //Get the source code text of target node
      auto templateName = TD->getNameAsString();
      auto instantiation = templateName + "<UndeclaredType>"; // Incorrect instantiation
      auto location = TD->getEndLoc();

      //Perform mutation on the source code text by applying string replacement
      std::string mutation = "/*mut55*/" + instantiation + " instance;";

      //Replace the original AST node with the mutated one
      Rewrite.InsertTextAfterToken(location, "\n" + mutation);
    }
}
  
void MutatorFrontendAction_55::MutatorASTConsumer_55::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateDecl().bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}