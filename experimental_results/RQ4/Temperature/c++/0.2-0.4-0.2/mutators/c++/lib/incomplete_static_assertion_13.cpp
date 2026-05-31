//source file
#include "../include/incomplete_static_assertion_13.h"

// ========================================================================================================
#define MUT13_OUTPUT 1

void MutatorFrontendAction_13::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *SA = Result.Nodes.getNodeAs<clang::StaticAssertDecl>("StaticAssert")) {
      //Filter nodes in header files
      if (!SA || !Result.Context->getSourceManager().isWrittenInMainFile(
                     SA->getLocation()))
        return;
      //Get the source code text of target node
      auto messageExpr = SA->getMessage();
      if (auto *SL = llvm::dyn_cast<clang::StringLiteral>(messageExpr)) {
        std::string originalMessage = SL->getString().str();
        if (!originalMessage.empty()) {
          //Perform mutation on the source code text by applying string replacement
          std::string mutatedMessage = originalMessage.substr(0, originalMessage.size() / 2);
          auto messageRange = SL->getSourceRange();
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(messageRange, "\"" + mutatedMessage + "\"");
        }
      }
    }
}
  
void MutatorFrontendAction_13::MutatorASTConsumer_13::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = staticAssertDecl().bind("StaticAssert");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}