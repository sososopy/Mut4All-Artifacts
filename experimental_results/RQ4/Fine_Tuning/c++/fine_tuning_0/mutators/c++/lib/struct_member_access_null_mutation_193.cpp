//source file
#include "../include/Struct_Member_Access_Null_Mutation_193.h"

// ========================================================================================================
#define MUT193_OUTPUT 1

void MutatorFrontendAction_193::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::MemberExpr>("MemberAccess")) {
      if (!MT || !Result.SourceManager->isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;

      auto member_name = stringutils::rangetoStr(*(Result.SourceManager),
                                                 MT->getSourceRange());
      llvm::outs() << member_name << '\n';
      string null_name = "/*mut193*/";
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()),
                          null_name);
    }
}
  
void MutatorFrontendAction_193::MutatorASTConsumer_193::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = memberExpr().bind("MemberAccess");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}