//source file
#include "../include/Replace_Template_Parameter_Pack_139.h"

// ========================================================================================================
#define MUT139_OUTPUT 1

void MutatorFrontendAction_139::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("ClassTemplateDecl")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isInMainFile(MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto declaration = clang::Lexer::getSourceText(
          CharSourceRange::getTokenRange(MT->getSourceRange()),
          *Result.SourceManager, Result.Context->getLangOpts());
      //Perform mutation on the source code text by applying string replacement
      // Replace the template parameter pack with a fixed set of types
      std::string replacement = "int, double, char";
      std::string declStr = declaration.str();
      size_t pos = declStr.find("...");
      if (pos != std::string::npos) {
        declStr.replace(pos, 3, replacement);
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declStr);
    }
}
  
void MutatorFrontendAction_139::MutatorASTConsumer_139::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = clang::ast_matchers::classTemplateDecl().bind("ClassTemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}