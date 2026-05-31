//source file
#include "../include/Mutator_TemplateParameterTypeChange_28.h"

// ========================================================================================================
#define MUT28_OUTPUT 1

void MutatorFrontendAction_28::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateTypeParmDecl>("TemplateParameter")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      //Replace 'typename' with 'class' or 'int' to change the template parameter type
      std::string mutatedDeclaration;
      if (declaration.find("typename") != std::string::npos) {
        mutatedDeclaration = declaration.replace(declaration.find("typename"), 8, "class");
      } else if (declaration.find("class") != std::string::npos) {
        mutatedDeclaration = declaration.replace(declaration.find("class"), 5, "int");
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedDeclaration);
    }
}
  
void MutatorFrontendAction_28::MutatorASTConsumer_28::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = templateTypeParmDecl().bind("TemplateParameter");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}