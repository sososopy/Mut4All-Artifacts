//source file
#include "../include/mutator_388.h"

// ========================================================================================================
#define MUT388_OUTPUT 1

void MutatorFrontendAction_388::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMemberCallExpr>("MemberCallExpr")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto call_text = stringutils::rangetoStr(*(Result.SourceManager),
                                               MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      if (call_text.find('(') != string::npos)
        call_text.erase(call_text.find('('));
      if (call_text.rfind(')') != string::npos)
        call_text.erase(call_text.rfind(')'));
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), call_text);
    }
}
  
void MutatorFrontendAction_388::MutatorASTConsumer_388::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = cxxMemberCallExpr().bind("MemberCallExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}