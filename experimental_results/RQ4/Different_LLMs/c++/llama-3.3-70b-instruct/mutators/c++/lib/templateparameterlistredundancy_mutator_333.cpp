//source file
#include "../include/Mutator_TemplateParameterListRedundancy_Mutator_333.h"

// ========================================================================================================
#define MUT333_OUTPUT 1

void MutatorFrontendAction_333::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateDecl>(("TemplateDecl"))) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = clang::Lexer::getSourceText(
          clang::CharSourceRange::getTokenRange(MT->getSourceRange()), 
          *Result.SourceManager, 
          Result.Context->getLangOpts());
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedDeclaration = std::string(declaration);
      size_t templateParamStart = mutatedDeclaration.find('<');
      size_t templateParamEnd = mutatedDeclaration.find('>');
      if (templateParamStart != std::string::npos && templateParamEnd != std::string::npos) {
        std::string templateParams = mutatedDeclaration.substr(templateParamStart + 1, templateParamEnd - templateParamStart - 1);
        if (templateParams.find("...") != std::string::npos) {
          // Introduce an additional parameter either before or after the pack
          size_t packPos = templateParams.find("...");
          if (packPos != std::string::npos) {
            std::string newParam = "class X";
            if (getrandom::getRandomIndex(1)) {
              // Insert before the pack
              templateParams.insert(packPos, newParam + ", ");
            } else {
              // Insert after the pack
              templateParams.insert(packPos + 3, ", " + newParam);
            }
          }
        } else {
          // Introduce a redundant or misplaced ellipsis
          if (getrandom::getRandomIndex(1)) {
            // Add an ellipsis at the beginning
            templateParams = "... " + templateParams;
          } else {
            // Add an ellipsis at the end
            templateParams += " ...";
          }
        }
        mutatedDeclaration.replace(templateParamStart + 1, templateParamEnd - templateParamStart - 1, templateParams);
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedDeclaration);
    }
}
  
void MutatorFrontendAction_333::MutatorASTConsumer_333::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = clang::ast_matchers::classTemplateDecl().bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}