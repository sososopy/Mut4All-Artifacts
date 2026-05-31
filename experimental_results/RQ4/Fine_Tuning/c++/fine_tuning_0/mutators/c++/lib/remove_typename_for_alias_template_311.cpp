//source file
#include "../include/Remove_Typename_For_Alias_Template_311.h"

// ========================================================================================================
#define MUT311_OUTPUT 1

void MutatorFrontendAction_311::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TypeAliasDecl>("Alias")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto alias = stringutils::rangetoStr(*(Result.SourceManager),
                                           MT->getSourceRange());
      auto alias_name = MT->getUnderlyingType().getTypePtr();
      if (!alias_name)
        return;
      if (alias_name->getTypeClass() != clang::Type::TypeClass::TemplateSpecialization)
        return;
      //Perform mutation on the source code text by applying string replacement
      alias = "/*mut311*/" + alias;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), alias);
    }
}
  
void MutatorFrontendAction_311::MutatorASTConsumer_311::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = clang::ast_matchers::typeAliasDecl(hasType(qualType(hasDescendant(templateSpecializationType())))).bind("Alias");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}