//source file
#include "../include/template_argument_type_mismatch_3.h"

// ========================================================================================================
#define MUT3_OUTPUT 1

void MutatorFrontendAction_3::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CTSI = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("TemplateInstance")) {
      //Filter nodes in header files
      if (!CTSI || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CTSI->getLocation()))
        return;
      
      //Check the number of template arguments
      if (CTSI->getTemplateArgs().size() < 2)
        return;
      
      //Get the source code text of target node
      auto templateArgs = CTSI->getTemplateArgs().asArray();
      std::string mutatedArg = "std::string";
      
      //Perform mutation on the source code text by applying string replacement
      std::string originalText = stringutils::rangetoStr(*(Result.SourceManager), CTSI->getSourceRange());
      size_t pos = originalText.find(templateArgs[1].getAsType().getAsString());
      if (pos != std::string::npos) {
        originalText.replace(pos, templateArgs[1].getAsType().getAsString().length(), mutatedArg);
      }
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(CTSI->getSourceRange()), originalText);
    }
}
  
void MutatorFrontendAction_3::MutatorASTConsumer_3::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateSpecializationDecl().bind("TemplateInstance");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}