//source file
#include "../include/Replace_String_Literal_With_Std_String_View_430.h"

// ========================================================================================================
#define MUT430_OUTPUT 1

void MutatorFrontendAction_430::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *SL = Result.Nodes.getNodeAs<clang::StringLiteral>("StringLiteral")) {
      //Filter nodes in header files
      if (!SL || !Result.Context->getSourceManager().isInMainFile(SL->getBeginLoc()))
        return;
      //Get the source code text of target node
      std::string replacement = "std::string_view(" + std::string(SL->getString()) + ")";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(SL->getSourceRange(), replacement);
    }
}

void MutatorFrontendAction_430::MutatorASTConsumer_430::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = stringLiteral().bind("StringLiteral");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}