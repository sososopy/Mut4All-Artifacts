//source file
#include "../include/Insert_Incomplete_Type_Alias_In_Template_181.h"

// ========================================================================================================
#define MUT181_OUTPUT 1

void MutatorFrontendAction_181::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateDecl")) {
      //Filter nodes in header files
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;
      //Get the source code text of target node
      auto templateDecl = TD->getTemplatedDecl();
      if (!templateDecl)
        return;

      auto sourceRange = templateDecl->getSourceRange();
      auto sourceText = stringutils::rangetoStr(*(Result.SourceManager), sourceRange);

      //Perform mutation on the source code text by applying string replacement
      std::string insertion = "\n\ttypename IncompleteType;";
      if (sourceText.find('{') != std::string::npos) {
          sourceText.insert(sourceText.find('{') + 1, insertion);
      }

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), sourceText);
    }
}
  
void MutatorFrontendAction_181::MutatorASTConsumer_181::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateDecl().bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}