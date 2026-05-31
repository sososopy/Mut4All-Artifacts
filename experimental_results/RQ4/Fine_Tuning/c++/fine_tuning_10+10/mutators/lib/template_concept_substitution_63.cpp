//source file
#include "../include/Template_Concept_Substitution_63.h"

// ========================================================================================================
#define MUT63_OUTPUT 1

void MutatorFrontendAction_63::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TTPT = Result.Nodes.getNodeAs<clang::TemplateTemplateParmDecl>("TemplateTemplateParam")) {
      //Filter nodes in header files
      if (!TTPT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TTPT->getLocation()))
        return;

      //Get the source code text of target node
      auto sourceRange = TTPT->getSourceRange();
      std::string originalText = stringutils::rangetoStr(*(Result.SourceManager), sourceRange);

      //Perform mutation on the source code text by applying string replacement
      std::string mutatedText = originalText;
      size_t conceptPos = mutatedText.find("<C>");
      if (conceptPos != std::string::npos) {
        mutatedText.replace(conceptPos, 3, "<typename>");
      }

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), mutatedText);
    }
}
  
void MutatorFrontendAction_63::MutatorASTConsumer_63::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = templateTemplateParmDecl().bind("TemplateTemplateParam");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}