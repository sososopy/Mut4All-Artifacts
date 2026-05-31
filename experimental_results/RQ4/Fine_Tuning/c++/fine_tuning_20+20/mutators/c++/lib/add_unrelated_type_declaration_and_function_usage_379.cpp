//source file
#include "../include/Add_Unrelated_Type_Declaration_And_Function_Usage_379.h"

// ========================================================================================================
#define MUT379_OUTPUT 1

void MutatorFrontendAction_379::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
        if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                       DL->getLocation()))
          return;
        if (DL->isMain())
          return;
        funcs.push_back(DL);
      } else if (auto *FD =
                     Result.Nodes.getNodeAs<clang::FunctionDecl>("Main")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FD->getLocation()))
          return;
        auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                               FD->getSourceRange());
        string typedef_decl = "typedef int mut_379;";
        if (content.find('{') != string::npos)
          content.insert(content.find('{') + 1, "/*mut379*/mut_379 a;");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FD->getSourceRange()), content);
        Rewrite.ReplaceText(FD->getBeginLoc(), 0, typedef_decl + '\n');
      }
}
  
void MutatorFrontendAction_379::MutatorASTConsumer_379::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionDecl().bind("Functions");
    auto main_matcher = functionDecl(isMain()).bind("Main");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(main_matcher, &callback);
    matchFinder.matchAST(Context);
}