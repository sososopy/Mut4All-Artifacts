//source file
#include "../include/add_unused_template_parameter_203.h"

// ========================================================================================================
#define MUT203_OUTPUT 1

void MutatorFrontendAction_203::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->isThisDeclarationADefinition()) {
        //Get the source code text of target node
        auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   FD->getSourceRange());
        //Perform mutation on the source code text by applying string replacement
        size_t pos = declaration.find('(');
        if (pos != std::string::npos) {
          std::string templateParam = "template<typename UnusedParam> ";
          declaration.insert(0, templateParam);
          declaration.insert(pos, "UnusedParam = int, ");
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), declaration);
        }
      }
    }
}
  
void MutatorFrontendAction_203::MutatorASTConsumer_203::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(isDefinition()).bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}