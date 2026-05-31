//source file
#include "../include/Lambda_with_Fold_Expression_Expansion_460.h"

// ========================================================================================================
#define MUT460_OUTPUT 1

void MutatorFrontendAction_460::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambda")) {
      if (!LE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LE->getBeginLoc()))
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             LE->getSourceRange());
      llvm::outs() << content << '\n';
      if (content.find("auto...") == string::npos)
        return;
      if (content.find("return") == string::npos)
        return;
      auto ret_pos = content.find("return");
      auto ret_end = content.find(';', ret_pos);
      auto ret_content = content.substr(ret_pos, ret_end - ret_pos + 1);
      llvm::outs() << ret_content << '\n';
      if (ret_content.find("...") == string::npos)
        return;
      if (ret_content.find(',') == string::npos)
        return;
      auto comma_pos = ret_content.find(',');
      auto ret_content_new = ret_content;
      ret_content_new.erase(comma_pos, 1);
      ret_content_new.insert(ret_content_new.find('(') + 1, "(");
      ret_content_new.insert(ret_content_new.find(')'),
                             ", 0) + "); // use plus
      llvm::outs() << ret_content_new << '\n';
      content.replace(ret_pos, ret_end - ret_pos + 1, ret_content_new);
      content = "/*mut460*/" + content;
      llvm::outs() << content << '\n';
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(LE->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_460::MutatorASTConsumer_460::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    StatementMatcher matcher = lambdaExpr().bind("Lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}