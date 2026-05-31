//source file
#include "../include/Remove_Semicolon_After_Typedef_Declaration_363.h"

// ========================================================================================================
#define MUT363_OUTPUT 1

void MutatorFrontendAction_363::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TypedefDecl>("TypedefDecl")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      llvm::outs() << declaration << '\n';
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(MT->getEndLoc(), 1, "");
    }
}
  
void MutatorFrontendAction_363::MutatorASTConsumer_363::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = typedefDecl().bind("TypedefDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}