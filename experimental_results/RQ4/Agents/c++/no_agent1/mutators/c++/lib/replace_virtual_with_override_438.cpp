//source file
#include "../include/replace_virtual_with_override_438.h"

// ========================================================================================================
#define MUT438_OUTPUT 1

void MutatorFrontendAction_438::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MD = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("Methods")) {
      //Filter nodes in header files
      if (!MD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MD->getLocation()))
        return;

      if (MD->isVirtual() && MD->size_overridden_methods() > 0) {
        //Get the source code text of target node
        auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MD->getSourceRange());
        //Perform mutation on the source code text by applying string replacement
        size_t pos = declaration.find("virtual");
        if (pos != std::string::npos) {
          declaration.replace(pos, 7, "override");
        }
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(MD->getSourceRange()), declaration);
      }
    }
}
  
void MutatorFrontendAction_438::MutatorASTConsumer_438::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxMethodDecl(isVirtual()).bind("Methods");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}