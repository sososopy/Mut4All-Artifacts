//source file
#include "../include/introduce_malformed_doc_comment_342.h"

// ========================================================================================================
#define MUT342_OUTPUT 1

void MutatorFrontendAction_342::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Get the source code text of target node
      auto functionText = stringutils::rangetoStr(*(Result.SourceManager), FD->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::string malformedComment = "/// Satisfies the [`Add<@example.self>`](example::Add) concept.\n";
      functionText = malformedComment + functionText;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), functionText);
    }
}
  
void MutatorFrontendAction_342::MutatorASTConsumer_342::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl().bind("FunctionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}