//source file
#include "../include/malform_struct_definition_358.h"

// ========================================================================================================
#define MUT358_OUTPUT 1

void MutatorFrontendAction_358::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ST = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Struct")) {
      if (!ST || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ST->getLocation()))
        return;
      if (!ST->isStruct())
        return;
      if (!ST->isCompleteDefinition())
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             ST->getSourceRange());
      if (content.rfind('}') != string::npos)
        content.erase(content.rfind('}'), 1);
      content += " struct mut358;";
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(ST->getSourceRange()),
                          content);
    }
}
  
void MutatorFrontendAction_358::MutatorASTConsumer_358::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Struct");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}