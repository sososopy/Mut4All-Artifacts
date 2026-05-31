//source file
#include "../include/insert_unreachable_code_275.h"

// ========================================================================================================
#define MUT275_OUTPUT 1

void MutatorFrontendAction_275::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->hasBody()) {
        //Get the source code text of target node
        auto body = FD->getBody();
        auto bodyStr = stringutils::rangetoStr(*(Result.SourceManager), body->getSourceRange());
        //Perform mutation on the source code text by applying string replacement
        auto unreachableCode = "\n/*mut275*/if (false) { __builtin_unreachable(); }\n";
        bodyStr.insert(bodyStr.find("{") + 1, unreachableCode);
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(body->getSourceRange()), bodyStr);
      }
    }
}
  
void MutatorFrontendAction_275::MutatorASTConsumer_275::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(isDefinition()).bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}