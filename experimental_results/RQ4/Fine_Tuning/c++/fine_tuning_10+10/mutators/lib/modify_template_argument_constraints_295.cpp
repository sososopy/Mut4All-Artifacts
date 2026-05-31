//source file
#include "../include/modify_template_argument_constraints_295.h"

// ========================================================================================================
#define MUT295_OUTPUT 1

void MutatorFrontendAction_295::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *NTTP = Result.Nodes.getNodeAs<clang::NonTypeTemplateParmDecl>("NonTypeTemplateParm")) {
      //Filter nodes in header files
      if (!NTTP || !Result.Context->getSourceManager().isWrittenInMainFile(
                     NTTP->getLocation()))
        return;

      //Get the source code text of target node
      auto sourceText = stringutils::rangetoStr(*(Result.SourceManager), NTTP->getSourceRange());
      
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedText = sourceText;
      if (sourceText.find("C") != std::string::npos) {
          mutatedText.replace(sourceText.find("C"), 1, "(C && sizeof...(auto) > 0)");
      }
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(NTTP->getSourceRange()), mutatedText);
    }
}
  
void MutatorFrontendAction_295::MutatorASTConsumer_295::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = nonTypeTemplateParmDecl(hasType(isInteger())).bind("NonTypeTemplateParm");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}