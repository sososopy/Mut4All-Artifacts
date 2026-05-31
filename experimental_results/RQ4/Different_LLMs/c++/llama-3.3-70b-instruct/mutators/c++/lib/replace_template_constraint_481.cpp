//source file
#include "../include/Replace_Template_Constraint_481.h"

// ========================================================================================================
#define REPLACE_TEMPLATE_CONSTRAINT_481_OUTPUT 1

void MutatorFrontendAction_481::Callback::run(const MatchFinder::MatchResult &Result) {
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
      // Replace std::is_constructible with std::is_convertible
      size_t pos = declaration.find("std::is_constructible");
      if (pos != string::npos) {
        declaration.replace(pos, 20, "std::is_convertible");
      }
      // Replace std::is_assignable with std::is_copy_assignable
      pos = declaration.find("std::is_assignable");
      if (pos != string::npos) {
        declaration.replace(pos, 17, "std::is_copy_assignable");
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_481::MutatorASTConsumer_481::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = clang::ast_matchers::classTemplateDecl().bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}