//source file
#include "../include/replace_virtual_with_pure_virtual_334.h"

// ========================================================================================================
#define MUT334_OUTPUT 1

void MutatorFrontendAction_334::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MD = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("Methods")) {
      //Filter nodes in header files
      if (!MD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MD->getLocation()))
        return;
      
      if (MD->isVirtual() && !MD->isPure()) {
        //Get the source code text of target node
        auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MD->getSourceRange());
        //Perform mutation on the source code text by applying string replacement
        if (declaration.find("virtual") != std::string::npos) {
          declaration.insert(declaration.rfind("}"), " = 0");
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(MD->getSourceRange()), declaration);
        }
      }
    }
}
  
void MutatorFrontendAction_334::MutatorASTConsumer_334::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxMethodDecl(isVirtual(), unless(isPure())).bind("Methods");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}