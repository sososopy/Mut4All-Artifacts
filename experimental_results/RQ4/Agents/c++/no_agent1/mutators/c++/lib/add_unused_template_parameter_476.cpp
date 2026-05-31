//source file
#include "../include/add_unused_template_parameter_476.h"

// ========================================================================================================
#define MUT476_OUTPUT 1

void MutatorFrontendAction_476::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->isTemplateInstantiation() || FD->isFunctionTemplateSpecialization())
        return;

      //Get the source code text of target node
      auto funcDecl = stringutils::rangetoStr(*(Result.SourceManager),
                                              FD->getSourceRange());

      //Perform mutation on the source code text by applying string replacement
      std::string templateStr = "template<typename UnusedParam>\n";
      funcDecl.insert(0, templateStr);

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), funcDecl);
    }
}
  
void MutatorFrontendAction_476::MutatorASTConsumer_476::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(isDefinition()).bind("FunctionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}