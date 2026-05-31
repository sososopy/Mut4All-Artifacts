//source file
#include "../include/Incomplete_Initialization_In_Static_Assert_208.h"

// ========================================================================================================
#define MUT208_OUTPUT 1

void MutatorFrontendAction_208::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::StaticAssertDecl>("StaticAssert")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto assert_content = stringutils::rangetoStr(*(Result.SourceManager),
                                                    MT->getSourceRange());
      llvm::outs() << assert_content << '\n';
      //Perform mutation on the source code text by applying string replacement
      int choice = getrandom::getRandomIndex(2);
      if (choice == 0) {
        assert_content = "static_assert(";
      } else if (choice == 1) {
        assert_content = "static_assert(" + assert_content + ",";
      } else {
        assert_content = "static_assert(" + assert_content + ",\"";
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), assert_content);
    }
}
  
void MutatorFrontendAction_208::MutatorASTConsumer_208::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = staticAssertDecl().bind("StaticAssert");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}