//source file
#include "../include/collapse_template_parameters_61.h"

// ========================================================================================================
#define MUT61_OUTPUT 1

void MutatorFrontendAction_61::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FuncTemplates")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      if (content.find("typename...") != string::npos) {
        stringutils::strReplace(content, "typename...", "typename");
        stringutils::strReplace(content, "args...", "args");
      }
      else if (content.find("class...") != string::npos) {
        stringutils::strReplace(content, "class...", "class");
        stringutils::strReplace(content, "args...", "args");
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_61::MutatorASTConsumer_61::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionTemplateDecl().bind("FuncTemplates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}