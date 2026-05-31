//source file
#include "../include/insert_unreachable_code_block_487.h"

// ========================================================================================================
#define MUT487_OUTPUT 1

void MutatorFrontendAction_487::Callback::run(const MatchFinder::MatchResult &Result) {
    // Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      // Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->hasBody()) {
        // Get the source code text of target node
        auto body = stringutils::rangetoStr(*(Result.SourceManager),
                                            FD->getBody()->getSourceRange());

        // Perform mutation on the source code text by applying string replacement
        std::string unreachableCode = "\n/*mut487*/if (false) { volatile int x = 0; x++; }\n";
        body.insert(body.find("{") + 1, unreachableCode);

        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), body);
      }
    }
}
  
void MutatorFrontendAction_487::MutatorASTConsumer_487::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(isDefinition()).bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}