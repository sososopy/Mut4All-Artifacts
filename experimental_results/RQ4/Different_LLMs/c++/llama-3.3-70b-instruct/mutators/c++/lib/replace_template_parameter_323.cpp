//source file
#include "../include/Replace_Template_Parameter_323.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/AST/DeclTemplate.h"

// ========================================================================================================
#define MUT323_OUTPUT 1

std::string getCompatibleType(const clang::TemplateTypeParmDecl *param, ASTContext &Context) {
  // This function should find a compatible type to replace the template parameter
  // For simplicity, let's assume we have a list of compatible types
  std::vector<std::string> compatibleTypes = {"int", "float", "double"};
  return compatibleTypes[getrandom::getRandomIndex(compatibleTypes.size() - 1)];
}

void MutatorFrontendAction_323::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateDecl>(("TemplateDecl"))) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                               MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      // Find all template parameters
      auto templateParams = MT->getTemplateParameters();
      if (templateParams && templateParams->size() > 0) {
        // Choose a random template parameter to replace
        int choice = getrandom::getRandomIndex(templateParams->size() - 1);
        auto param = templateParams->getParam(choice);
        if (auto *templateTypeParmDecl = dyn_cast<clang::TemplateTypeParmDecl>(param)) {
          // Find a compatible type to replace the template parameter
          auto compatibleType = ::getCompatibleType(templateTypeParmDecl, *Result.Context);
          if (!compatibleType.empty()) {
            // Replace the template parameter with the compatible type
            declaration.replace(declaration.find(param->getNameAsString()),
                              param->getNameAsString().size(), compatibleType);
            //Replace the original AST node with the mutated one
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
          }
        }
      }
    }
}
  
void MutatorFrontendAction_323::MutatorASTConsumer_323::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = clang::ast_matchers::classTemplateDecl().bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}