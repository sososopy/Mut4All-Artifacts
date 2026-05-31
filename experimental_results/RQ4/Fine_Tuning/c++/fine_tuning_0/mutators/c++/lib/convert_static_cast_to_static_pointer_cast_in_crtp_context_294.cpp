//source file
#include "../include/Convert_Static_Cast_To_Static_Pointer_Cast_In_CRTP_Context_294.h"

// ========================================================================================================
#define MUT294_OUTPUT 1

void MutatorFrontendAction_294::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMemberCallExpr>("MemberCallExpr")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getExprLoc()))
        return;
      //Get the source code text of target node
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      auto pos1 = content.find("static_cast<");
      auto pos2 = content.find(">(*this)");
      if (pos1 == string::npos || pos2 == string::npos)
        return;
      auto target = content.substr(pos1, pos2 - pos1 + 9);
      auto target_new = target;
      target_new.insert(target_new.find_last_of('>'), " *");
      target_new.replace(target_new.find(">(*this)"), 9, "(this)");
      content.replace(content.find(target), target.length(), target_new);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_294::MutatorASTConsumer_294::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = cxxMemberCallExpr().bind("MemberCallExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}