//source file
#include "../include/insert_recursive_function_call_121.h"

// ========================================================================================================
#define MUT121_OUTPUT 1

void MutatorFrontendAction_121::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->hasBody() && !FD->isMain()) {
        //Get the source code text of target node
        auto funcBody = stringutils::rangetoStr(*(Result.SourceManager),
                                                FD->getBody()->getSourceRange());
        //Perform mutation on the source code text by applying string replacement
        std::string recursiveCall = FD->getNameAsString() + "(); /*mut121*/";
        funcBody.insert(funcBody.find("{") + 1, "\n" + recursiveCall + "\n");
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getBody()->getSourceRange()), funcBody);
      }
    }
}
  
void MutatorFrontendAction_121::MutatorASTConsumer_121::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(unless(isMain())).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}