//source file
#include "../include/invalid_incomplete_struct_member_access_365.h"

// ========================================================================================================
#define MUT365_OUTPUT 1

void MutatorFrontendAction_365::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::MemberExpr>("MemberAccess")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto membername = stringutils::rangetoStr(*(Result.SourceManager),
                                                MT->getMemberNameInfo().getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      auto newname = "invalid_field_name";
      llvm::outs() << "mut365: " << membername << "->" << newname << '\n';
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(MT->getMemberNameInfo().getSourceRange(), newname);
    }
}
  
void MutatorFrontendAction_365::MutatorASTConsumer_365::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = memberExpr().bind("MemberAccess");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}