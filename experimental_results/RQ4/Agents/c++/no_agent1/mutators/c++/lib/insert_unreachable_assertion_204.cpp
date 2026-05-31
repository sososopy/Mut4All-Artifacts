//source file
#include "../include/insert_unreachable_assertion_204.h"

// ========================================================================================================
#define MUT204_OUTPUT 1

void MutatorFrontendAction_204::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FS = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      //Filter nodes in header files
      if (!FS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FS->getLocation()))
        return;

      if (FS->hasBody()) {
        //Get the source code text of target node
        auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   FS->getSourceRange());
        //Perform mutation on the source code text by applying string replacement
        declaration.insert(declaration.find("{") + 1, "\n/*mut204*/assert(false && \"Unreachable code\");\n");
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FS->getSourceRange()), declaration);
      }
    }
}
  
void MutatorFrontendAction_204::MutatorASTConsumer_204::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(hasBody(stmt())).bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}