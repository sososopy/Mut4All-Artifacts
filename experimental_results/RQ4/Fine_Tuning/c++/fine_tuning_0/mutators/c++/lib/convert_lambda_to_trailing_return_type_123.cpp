//source file
#include "../include/Convert_Lambda_To_Trailing_Return_Type_123.h"

// ========================================================================================================
#define MUT123_OUTPUT 1

void MutatorFrontendAction_123::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("LambdaExpr")) {
      if (!LE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LE->getBeginLoc()))
        return;
      if (LE->hasExplicitParameters() == false)
        return;
      if (LE->hasExplicitResultType() == true)
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             LE->getSourceRange());
      llvm::outs() << content << '\n';
      auto body = LE->getBody();
      auto body_content = stringutils::rangetoStr(*(Result.SourceManager),
                                                  body->getSourceRange());
      string ret_type = body_content;
      auto pos = ret_type.find("return");
      if (pos == string::npos)
        return;
      ret_type = ret_type.substr(pos + 6);
      ret_type = ret_type.substr(0, ret_type.find(";"));
      ret_type = "->decltype(" + ret_type + ")";
      content.insert(content.find("{"), ret_type);
      llvm::outs() << content << '\n';
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(LE->getSourceRange()),
                          content);
    }
}
  
void MutatorFrontendAction_123::MutatorASTConsumer_123::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = lambdaExpr().bind("LambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}