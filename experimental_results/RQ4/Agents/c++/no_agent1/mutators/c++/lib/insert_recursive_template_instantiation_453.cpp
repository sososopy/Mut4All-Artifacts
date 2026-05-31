//source file
#include "../include/insert_recursive_template_instantiation_453.h"

// ========================================================================================================
#define MUT453_OUTPUT 1

void MutatorFrontendAction_453::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateDecl")) {
      //Filter nodes in header files
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;

      //Get the source code text of target node
      auto templateName = TD->getNameAsString();
      if (templateName.empty())
        return;

      //Perform mutation on the source code text by applying string replacement
      std::string recursiveTemplate = "template<int N>\nclass RecursiveTemplate_" + templateName + " {\n"
                                      "public:\n"
                                      "    static const int value = RecursiveTemplate_" + templateName + "<N-1>::value;\n"
                                      "};\n"
                                      "template<>\nclass RecursiveTemplate_" + templateName + "<0> {\n"
                                      "public:\n"
                                      "    static const int value = 0;\n"
                                      "};\n";

      //Replace the original AST node with the mutated one
      Rewrite.InsertTextAfterToken(TD->getEndLoc(), "\n/*mut453*/" + recursiveTemplate);
    }
}
  
void MutatorFrontendAction_453::MutatorASTConsumer_453::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateDecl().bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}