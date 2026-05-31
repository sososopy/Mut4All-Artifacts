//source file
#include "../include/insert_unreachable_if_statement_192.h"

// ========================================================================================================
#define MUT192_OUTPUT 1

void MutatorFrontendAction_192::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->hasBody()) {
        //Get the source code text of target node
        auto body = FD->getBody();
        auto bodyText = stringutils::rangetoStr(*(Result.SourceManager), body->getSourceRange());
        //Perform mutation on the source code text by applying string replacement
        std::string unreachableIf = "\n/*mut192*/if (false) { __builtin_unreachable(); }\n";
        bodyText.insert(bodyText.find("{") + 1, unreachableIf);
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(body->getSourceRange()), bodyText);
      }
    }
}
  
void MutatorFrontendAction_192::MutatorASTConsumer_192::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(hasBody(stmt())).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}