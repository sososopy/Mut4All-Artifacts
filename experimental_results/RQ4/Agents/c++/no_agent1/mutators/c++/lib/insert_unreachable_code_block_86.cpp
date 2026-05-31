//source file
#include "../include/insert_unreachable_code_block_86.h"

// ========================================================================================================
#define MUT86_OUTPUT 1

void MutatorFrontendAction_86::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (!FD->hasBody())
        return;

      auto body = FD->getBody();
      if (!body)
        return;

      auto bodyStr = stringutils::rangetoStr(*(Result.SourceManager),
                                             body->getSourceRange());
      // Perform mutation by inserting an unreachable code block
      std::string unreachableBlock = "\n/*mut86*/if (false) { while(true) {} }\n";
      bodyStr.insert(bodyStr.find("{") + 1, unreachableBlock);
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(body->getSourceRange()), bodyStr);
    }
}
  
void MutatorFrontendAction_86::MutatorASTConsumer_86::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = functionDecl(hasBody(stmt())).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}