//source file
#include "../include/Replace_Template_Parameter_With_Incomplete_Type_406.h"

// ========================================================================================================
#define MUT406_OUTPUT 1

void MutatorFrontendAction_406::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Template")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::string incompleteType = "class IncompleteType;";
      declaration.insert(0, incompleteType);
      std::string templateParameter = MT->getTemplateParameters()->getParam(0)->getNameAsString();
      size_t pos = declaration.find(templateParameter);
      if (pos != std::string::npos) {
        declaration.replace(pos, templateParameter.length(), "IncompleteType");
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_406::MutatorASTConsumer_406::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateDecl().bind("Template");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}