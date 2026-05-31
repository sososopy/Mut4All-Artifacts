//source file
#include "../include/Modify_Function_Call_With_Template_Instantiation_388.h"

// ========================================================================================================
#define MUT388_OUTPUT 1

void MutatorFrontendAction_388::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMemberCallExpr>("MemberTemplateCall")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto callee = MT->getCallee();
      auto callee_name = stringutils::rangetoStr(*(Result.SourceManager),
                                                 callee->getSourceRange());
      llvm::outs() << callee_name << '\n';
      //Perform mutation on the source code text by applying string replacement
      callee_name = "/*mut388*/" + callee_name;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), callee_name);
    }
}
  
void MutatorFrontendAction_388::MutatorASTConsumer_388::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = cxxMemberCallExpr(callee(cxxDependentScopeMemberExpr())).bind("MemberTemplateCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}