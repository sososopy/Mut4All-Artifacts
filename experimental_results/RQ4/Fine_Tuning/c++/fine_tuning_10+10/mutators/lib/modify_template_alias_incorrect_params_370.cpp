//source file
#include "../include/modify_template_alias_incorrect_params_370.h"

// ========================================================================================================
#define MUT370_OUTPUT 1

void MutatorFrontendAction_370::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TA = Result.Nodes.getNodeAs<clang::TypeAliasTemplateDecl>("TemplateAlias")) {
      //Filter nodes in header files
      if (!TA || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TA->getLocation()))
        return;
      //Get the source code text of target node
      auto aliasDecl = TA->getTemplatedDecl();
      std::string aliasText = stringutils::rangetoStr(*(Result.SourceManager), aliasDecl->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedText = "template <typename Unused> " + aliasText;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(aliasDecl->getSourceRange()), mutatedText);
    }
}
  
void MutatorFrontendAction_370::MutatorASTConsumer_370::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = typeAliasTemplateDecl().bind("TemplateAlias");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}