//source file
#include "../include/Mutator_Modify_Template_Parameter_Type_22.h"
#include "clang/ASTMatchers/ASTMatchers.h"

// ========================================================================================================
#define MUT22_OUTPUT 1

void MutatorFrontendAction_22::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateDecl>(("TemplateDecl"))) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = llvm::StringRef(MT->getSourceRange().getBegin().printToString(*Result.SourceManager)).str();
      //Perform mutation on the source code text by applying string replacement
      //Replace the original AST node with the mutated one
      std::string mutatedDeclaration = declaration;
      // Find the template parameter type
      size_t templateParamStart = declaration.find("template <");
      size_t templateParamEnd = declaration.find(">", templateParamStart);
      if (templateParamStart != std::string::npos && templateParamEnd != std::string::npos) {
        std::string templateParam = declaration.substr(templateParamStart + 9, templateParamEnd - templateParamStart - 9);
        // Replace the template parameter type with a similar type
        size_t spacePos = templateParam.find(" ");
        if (spacePos != std::string::npos) {
          std::string newTemplateParam = "int";
          mutatedDeclaration.replace(templateParamStart + 9, spacePos, newTemplateParam);
        }
      }
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedDeclaration);
    }
}
  
void MutatorFrontendAction_22::MutatorASTConsumer_22::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = clang::ast_matchers::classTemplateDecl().bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}