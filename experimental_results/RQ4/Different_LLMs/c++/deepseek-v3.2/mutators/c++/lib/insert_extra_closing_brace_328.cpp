//source file
#include "../include/Mutator_328.h"

// ========================================================================================================
#define MUT328_OUTPUT 1

void MutatorFrontendAction_328::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("ClassDecl")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      if (!MT->isCompleteDefinition())
        return;
      //Get the source code text of target node
      SourceLocation endLoc = MT->getEndLoc();
      //Perform mutation on the source code text by applying string replacement
      //Insert an extra closing brace after the class definition
      Rewrite.InsertTextAfterToken(endLoc, "}");
      //Replace the original AST node with the mutated one
    }
}
  
void MutatorFrontendAction_328::MutatorASTConsumer_328::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl(isDefinition()).bind("ClassDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}