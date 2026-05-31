//source file
#include "../include/modify_member_access_43.h"

// ========================================================================================================
#define MUT43_OUTPUT 1

void MutatorFrontendAction_43::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::MemberExpr>("MemberExpr")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getExprLoc()))
        return;

      //Get the source code text of target node
      auto memberName = MT->getMemberDecl()->getNameAsString();
      auto baseExpr = MT->getBase();
      auto baseType = baseExpr->getType().getAsString();

      //Perform mutation on the source code text by applying string replacement
      std::string mutatedMemberAccess = baseType + ".nonExistentMember";
      if (MT->isArrow()) {
        mutatedMemberAccess = baseType + "->nonExistentMember";
      }

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(MT->getSourceRange(), mutatedMemberAccess);
    }
}

void MutatorFrontendAction_43::MutatorASTConsumer_43::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = memberExpr().bind("MemberExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}