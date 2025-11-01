//source file
#include "../include/Template_Argument_Mismatch_In_Aliases_369.h"

// ========================================================================================================
#define MUT369_OUTPUT 1

void MutatorFrontendAction_369::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *AliasDecl = Result.Nodes.getNodeAs<clang::TypeAliasTemplateDecl>("AliasTemplate")) {
      //Filter nodes in header files
      if (!AliasDecl || !Result.Context->getSourceManager().isWrittenInMainFile(
                     AliasDecl->getLocation()))
        return;

      // Get the source code text of target node
      auto aliasUsage = Result.Nodes.getNodeAs<clang::TypeAliasDecl>("AliasUsage");
      if (!aliasUsage)
        return;

      //Perform mutation on the source code text by applying string replacement
      auto aliasName = AliasDecl->getNameAsString();
      std::string modifiedUsage = aliasName + "<int>"; // Introduce mismatch by removing template template parameter

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(aliasUsage->getSourceRange(), modifiedUsage);
    }
}
  
void MutatorFrontendAction_369::MutatorASTConsumer_369::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto aliasTemplateMatcher = typeAliasTemplateDecl().bind("AliasTemplate");
    auto aliasUsageMatcher = typeAliasDecl(hasType(type().bind("AliasUsage"))).bind("AliasUsage");

    Callback callback(TheRewriter);
    matchFinder.addMatcher(aliasTemplateMatcher, &callback);
    matchFinder.addMatcher(aliasUsageMatcher, &callback);
    matchFinder.matchAST(Context);
}