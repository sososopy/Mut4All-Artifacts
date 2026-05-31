//source file
#include "../include/constexpr_incomplete_struct_5.h"

// ========================================================================================================
#define MUT5_OUTPUT 1

void MutatorFrontendAction_5::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *SD = Result.Nodes.getNodeAs<clang::RecordDecl>("IncompleteStruct")) {
      //Filter nodes in header files
      if (!SD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     SD->getLocation()))
        return;
      if (SD->isCompleteDefinition())
        return;
      //Get the source code text of target node
      auto structName = SD->getNameAsString();
      std::string constexprDecl = "constexpr " + structName + " myVar = {};";
      //Perform mutation on the source code text by applying string replacement
      SourceLocation insertLoc = SD->getEndLoc().getLocWithOffset(1);
      //Replace the original AST node with the mutated one
      Rewrite.InsertText(insertLoc, "\n/*mut5*/" + constexprDecl);
    }
}
  
void MutatorFrontendAction_5::MutatorASTConsumer_5::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = recordDecl(unless(isDefinition())).bind("IncompleteStruct");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}