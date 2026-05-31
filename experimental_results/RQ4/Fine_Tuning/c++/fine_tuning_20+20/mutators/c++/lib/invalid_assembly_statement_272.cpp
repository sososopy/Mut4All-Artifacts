//source file
#include "../include/Invalid_Assembly_Statement_272.h"

// ========================================================================================================
#define MUT272_OUTPUT 1

void MutatorFrontendAction_272::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::GCCAsmStmt>("Asms")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getAsmLoc()))
        return;
      //Get the source code text of target node
      auto assembly = stringutils::rangetoStr(*(Result.SourceManager),
                                              MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      assembly = "/*mut272*/asm(\"invalid_syntax\" : \"=r\"(out) : \"r\"(in));";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), assembly);
    }
}
  
void MutatorFrontendAction_272::MutatorASTConsumer_272::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = ast_matchers::asmStmt().bind("Asms");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}