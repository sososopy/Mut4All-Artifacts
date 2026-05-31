//source file
#include "../include/add_default_argument_with_uninstantiated_inline_call_247.h"

// ========================================================================================================
#define MUT247_OUTPUT 1

void MutatorFrontendAction_247::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getNumParams() == 0)
        return;
      candidate_funcs.push_back(FD);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>(
                   "DefaultedFunctions")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->getNumParams() == 0)
        return;
      // llvm::outs() << DL->getNameAsString() << '\n';
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      auto params = DL->parameters();
      for (auto param : params) {
        if (param->hasDefaultArg()) {
          auto argname = param->getNameAsString();
          auto defaultarg = argname + "()";
          auto defaultcontent = stringutils::rangetoStr(
              *(Result.SourceManager), param->getDefaultArgRange());
          llvm::outs() << defaultcontent << '\n';
          stringutils::strReplace(content, defaultcontent, defaultarg);
        }
      }
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()),
                          content);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>(
                   "NondefaultedFunctions")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->getNumParams() == 0)
        return;
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      auto params = DL->parameters();
      int dice = getrandom::getRandomIndex(params.size() - 1);
      auto argname = params[dice]->getNameAsString();
      auto defaultarg = argname + "()";
      if (content.rfind(')') != string::npos)
        content.insert(content.rfind(')'), "=" + defaultarg);
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()),
                          content);
    }
  }

void MutatorFrontendAction_247::MutatorASTConsumer_247::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionDecl().bind("Functions");
    auto defaulted_matcher =
        functionDecl(hasAnyParameter(parmVarDecl(hasDefaultArgument())))
            .bind("DefaultedFunctions");
    auto nondefaulted_matcher =
        functionDecl(unless(hasAnyParameter(parmVarDecl(hasDefaultArgument()))))
            .bind("NondefaultedFunctions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(defaulted_matcher, &callback);
    matchFinder.addMatcher(nondefaulted_matcher, &callback);
    matchFinder.matchAST(Context);
}