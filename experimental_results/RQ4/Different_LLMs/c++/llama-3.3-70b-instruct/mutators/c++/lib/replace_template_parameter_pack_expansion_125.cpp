//source file
#include "../include/Replace_Template_Parameter_Pack_Expansion_125.h"

// ========================================================================================================
#define MUT125_OUTPUT 1

void MutatorFrontendAction_125::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      // Introduce a new template parameter pack 'Vs'
      size_t pos = declaration.find("typename... Ts");
      if (pos != string::npos) {
        declaration.insert(pos + 17, ", typename... Vs");
      }
      pos = declaration.find("Ts");
      if (pos != string::npos) {
        declaration.insert(pos, "Vs, ");
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_125::MutatorASTConsumer_125::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionTemplateDecl().bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}