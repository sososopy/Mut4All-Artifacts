//source file
#include "../include/Add_Recursive_Function_Call_432.h"

// ========================================================================================================
#define MUT432_OUTPUT 1

void MutatorFrontendAction_432::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->isTemplateInstantiation())
        return;
      if (!FD->hasBody())
        return;
      auto name = FD->getNameAsString();
      if (name == "")
        return;
      cur_functions.push_back(FD);
    } else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>(
                   "MutatingFunction")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->isTemplateInstantiation())
        return;
      if (!FD->hasBody())
        return;
      auto name = FD->getNameAsString();
      if (name == "")
        return;
      auto params = FD->parameters();
      std::string call = name + "(";
      if (params.size() > 0) {
        for (std::size_t i = 0; i < params.size() - 1; ++i) {
          call += params[i]->getNameAsString() + ",";
        }
        call += params[params.size() - 1]->getNameAsString();
      }
      call += ");";
      llvm::outs() << call << '\n';
      auto body = FD->getBody();
      auto body_content = stringutils::rangetoStr(*(Result.SourceManager),
                                                  body->getSourceRange());
      body_content.insert(body_content.find("{") + 1, "/*mut432*/" + call);
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(body->getSourceRange()),
                          body_content);
    }
  }

void MutatorFrontendAction_432::MutatorASTConsumer_432::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionDecl().bind("Functions");
    auto mutating_matcher = functionDecl().bind("MutatingFunction");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(mutating_matcher, &callback);
    matchFinder.matchAST(Context);
}