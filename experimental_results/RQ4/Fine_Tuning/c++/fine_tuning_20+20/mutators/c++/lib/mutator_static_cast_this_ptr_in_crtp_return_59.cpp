//source file
#include "../include/Mutator_Static_Cast_This_Ptr_In_Crtp_Return_59.h"

// ========================================================================================================
#define MUT59_OUTPUT 1

void MutatorFrontendAction_59::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXStaticCastExpr>("StaticCast")) {
      //Filter nodes in header files
      if (!MT || !Result.SourceManager->isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      if (text.find("static_cast") != string::npos)
        text.replace(text.find("static_cast"), 11, "/*mut59*/dynamic_cast");
      else
        return;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), text);
    }
}
  
void MutatorFrontendAction_59::MutatorASTConsumer_59::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = cxxStaticCastExpr(
                                     hasAncestor(returnStmt(hasReturnValue(cxxThisExpr()))))
                                     .bind("StaticCast");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}