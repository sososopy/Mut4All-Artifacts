//source file
#include "../include/Modify_Template_Parameter_Pack_113.h"

// ========================================================================================================
#define MUT113_OUTPUT 1

void MutatorFrontendAction_113::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TA = Result.Nodes.getNodeAs<clang::TemplateSpecializationType>("TemplateAlias")) {
      //Filter nodes in header files
      if (!TA || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TA->getTemplateName().getAsTemplateDecl()->getLocation()))
        return;

      // Get the source code text of target node
      auto templateText = stringutils::rangetoStr(*(Result.SourceManager), TA->getTemplateName().getAsTemplateDecl()->getSourceRange());

      // Perform mutation on the source code text by applying string replacement
      // Introduce an additional template argument to the parameter pack expansion
      size_t pos = templateText.find(">");
      if (pos != std::string::npos) {
          templateText.insert(pos, ", 999");
      }

      // Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(TA->getTemplateName().getAsTemplateDecl()->getSourceRange()), templateText);
    }
}
  
void MutatorFrontendAction_113::MutatorASTConsumer_113::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define one or more ASTMatchers to identify the target AST node for mutation.
    TypeMatcher matcher = templateSpecializationType().bind("TemplateAlias");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}