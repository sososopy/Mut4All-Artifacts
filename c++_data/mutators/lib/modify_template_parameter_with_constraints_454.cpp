//source file
#include "../include/modify_template_parameter_with_constraints_454.h"

// ========================================================================================================
#define MUT454_OUTPUT 1

void MutatorFrontendAction_454::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *NTTP = Result.Nodes.getNodeAs<clang::TemplateTypeParmDecl>("NTTP")) {
      //Filter nodes in header files
      if (!NTTP || !Result.Context->getSourceManager().isWrittenInMainFile(
                     NTTP->getLocation()))
        return;
      
      //Get the source code text of target node
      auto range = NTTP->getSourceRange();
      std::string originalText = stringutils::rangetoStr(*(Result.SourceManager), range);
      
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedText = "auto"; // Remove the constraint
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(range, mutatedText);
    }
}
  
void MutatorFrontendAction_454::MutatorASTConsumer_454::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = templateTypeParmDecl().bind("NTTP");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}