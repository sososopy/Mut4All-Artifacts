//source file
#include "../include/Erroneous_Template_Parameter_List_In_Alias_Template_131.h"

// ========================================================================================================
#define MUT131_OUTPUT 1

void MutatorFrontendAction_131::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *ATD = Result.Nodes.getNodeAs<clang::TypeAliasTemplateDecl>("aliasTemplate")) {
      //Filter nodes in header files
      if (!ATD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ATD->getLocation()))
        return;
      //Get the source code text of target node
      auto aliasText = stringutils::rangetoStr(*(Result.SourceManager), ATD->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::string erroneousTemplate = "\ntemplate <typename U> ";
      aliasText.insert(aliasText.find("using"), erroneousTemplate);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(ATD->getSourceRange()), aliasText);
    }
}
  
void MutatorFrontendAction_131::MutatorASTConsumer_131::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = typeAliasTemplateDecl().bind("aliasTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}