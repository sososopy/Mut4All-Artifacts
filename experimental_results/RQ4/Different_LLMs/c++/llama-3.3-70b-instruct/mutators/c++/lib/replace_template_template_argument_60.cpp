//source file
#include "../include/Replace_Template_Template_Argument_60.h"

// ========================================================================================================
#define MUT60_OUTPUT 1

void MutatorFrontendAction_60::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateTemplateParmDecl>("TemplateTemplateParm")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      // Replace template template argument with a new template that has the same constraints but with a different template parameter list
      std::string mutatedDeclaration = declaration;
      size_t pos = mutatedDeclaration.find("template<");
      if (pos != std::string::npos) {
        size_t endPos = mutatedDeclaration.find(">", pos);
        if (endPos != std::string::npos) {
          std::string templateParams = mutatedDeclaration.substr(pos + 9, endPos - pos - 9);
          std::string newTemplateParams = "C int...";
          mutatedDeclaration.replace(pos + 9, templateParams.length(), newTemplateParams);
        }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedDeclaration);
    }
}
  
void MutatorFrontendAction_60::MutatorASTConsumer_60::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = templateTemplateParmDecl().bind("TemplateTemplateParm");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}