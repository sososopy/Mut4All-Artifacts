//source file
#include "../include/add_unused_template_parameter_394.h"

// ========================================================================================================
#define MUT394_OUTPUT 1

void MutatorFrontendAction_394::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateDecl")) {
      //Filter nodes in header files
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;

      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 TD->getSourceRange());

      //Perform mutation on the source code text by applying string replacement
      size_t templatePos = declaration.find("template <");
      if (templatePos != std::string::npos) {
          size_t insertPos = declaration.find('>', templatePos);
          if (insertPos != std::string::npos) {
              declaration.insert(insertPos, ", typename UnusedParam /*mut394*/");
          }
      }

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(TD->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_394::MutatorASTConsumer_394::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateDecl().bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}