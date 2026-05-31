//source file
#include "../include/Replace_Template_Parameter_With_Incompatible_Type_365.h"

// ========================================================================================================
#define MUT365_OUTPUT 1

void MutatorFrontendAction_365::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("TemplateInstantiation")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      // Replace template parameter with an incompatible type (e.g., int with std::string)
      std::string incompatibleType = "std::string";
      if (MT->getTemplateArgs().size() > 0) {
          auto templateArg = MT->getTemplateArgs().get(0);
          if (templateArg.getKind() == TemplateArgument::Type) {
              auto type = templateArg.getAsType();
              std::string templateParameter = type.getAsString();
              declaration.replace(declaration.find(templateParameter), templateParameter.length(), incompatibleType);
          }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_365::MutatorASTConsumer_365::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateSpecializationDecl().bind("TemplateInstantiation");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}