//source file
#include "../include/Mutator_Modify_Template_Parameter_List_33.h"
#include "clang/ASTMatchers/ASTMatchers.h"

// ========================================================================================================
#define MUT33_OUTPUT 1

void MutatorFrontendAction_33::Callback::run(const MatchFinder::MatchResult &Result) {
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
      // Adding a new template parameter to the list
      std::string newParam = "typename NewType";
      size_t pos = declaration.find(">");
      if (pos != std::string::npos) {
        declaration.insert(pos, ", " + newParam);
      }
      // Removing an existing template parameter from the list
      // Reordering the template parameters in the list
      // Changing the type of an existing template parameter
      std::string modifiedParam = "int";
      pos = declaration.find("typename");
      if (pos != std::string::npos) {
        declaration.replace(pos, 8, modifiedParam);
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_33::MutatorASTConsumer_33::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = clang::ast_matchers::classTemplateDecl().bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}