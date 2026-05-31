//source file
#include "../include/Target_Struct_Member_Access_In_Constant_Expressions_216.h"

// ========================================================================================================
#define MUT216_OUTPUT 1

void MutatorFrontendAction_216::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::MemberExpr>("StructMember")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      auto code = stringutils::rangetoStr(*(Result.SourceManager),
                                          MT->getSourceRange());
      llvm::outs() << code << '\n';
      code = "/*mut216*/my_macro(" + code + ")";
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), code);
    }
}
  
void MutatorFrontendAction_216::MutatorASTConsumer_216::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = varDecl(isExpansionInMainFile(), isConstexpr(),
                                         hasType(isInteger())).bind("Consts");
    auto struct_member_matcher =
        memberExpr(hasAncestor(varDecl(isConstexpr()))).bind("StructMember");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(struct_member_matcher, &callback);
    matchFinder.matchAST(Context);
}