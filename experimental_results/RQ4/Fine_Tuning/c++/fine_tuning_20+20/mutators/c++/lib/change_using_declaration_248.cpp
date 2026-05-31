//source file
#include "../include/change_using_declaration_248.h"

// ========================================================================================================
#define MUT248_OUTPUT 1

void MutatorFrontendAction_248::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *UD = Result.Nodes.getNodeAs<clang::UsingDecl>("UsingDecls")) {
      if (!UD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     UD->getLocation()))
        return;
      using_decls.push_back(UD);
    } else if (auto *DL =
                   Result.Nodes.getNodeAs<clang::UsingDecl>("UsingDecl")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (using_decls.empty())
        return;
      size_t index = getrandom::getRandomIndex(using_decls.size() - 1);
      auto target = using_decls[index];
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), target->getSourceRange());
      llvm::outs() << content << '\n';
      if (content.find('=') == string::npos)
        return;
      auto left = content.substr(0, content.find('='));
      string right = "MyUndefinedType";
      auto choice = getrandom::getRandomIndex(1);
      if (choice == 0)
        right = "MyUndefinedType";
      else
        right = "MyUndefinedFunction";
      auto mutated = left + "=/*mut248*/" + right;
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(target->getSourceRange()), mutated);
    }
}
  
void MutatorFrontendAction_248::MutatorASTConsumer_248::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = usingDecl().bind("UsingDecls");
    auto matcher2 = usingDecl().bind("UsingDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(matcher2, &callback);
    matchFinder.matchAST(Context);
}