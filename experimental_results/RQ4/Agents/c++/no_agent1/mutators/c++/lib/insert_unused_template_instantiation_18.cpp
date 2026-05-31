//source file
#include "../include/insert_unused_template_instantiation_18.h"

// ========================================================================================================
#define MUT18_OUTPUT 1

void MutatorFrontendAction_18::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      //Get the source code text of target node
      auto functionCode = stringutils::rangetoStr(*(Result.SourceManager),
                                                  FD->getSourceRange());

      //Perform mutation by adding a template instantiation
      std::string templateCode = "\n/*mut18*/ template<typename T> struct UnusedTemplate { T value; };";
      templateCode += "\nUnusedTemplate<int> unusedInstance;";
      
      //Insert the template instantiation at the beginning of the function body
      if (FD->hasBody()) {
        functionCode.insert(functionCode.find("{") + 1, templateCode);
      }

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), functionCode);
    }
}
  
void MutatorFrontendAction_18::MutatorASTConsumer_18::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = functionDecl(isDefinition()).bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}