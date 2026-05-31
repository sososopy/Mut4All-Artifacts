//source file
#include "../include/recursive_template_lambda_267.h"

// ========================================================================================================
#define MUT267_OUTPUT 1

void MutatorFrontendAction_267::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Templates")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (!FD->isFunctionTemplateSpecialization() &&
          !FD->getDescribedFunctionTemplate())
        return;
      if (!FD->hasBody())
        return;
      auto body = FD->getBody();
      auto bodystr = stringutils::rangetoStr(*(Result.SourceManager),
                                             body->getSourceRange());
      llvm::outs() << bodystr << '\n';
      if (bodystr.find("return") != string::npos) {
        bodystr = bodystr.substr(0, bodystr.rfind("return")) +
                  "/*mut267*/return [&]{return " + FD->getNameAsString() +
                  "(0);}();\n";
      } else {
        bodystr = bodystr.substr(0, bodystr.rfind('}')) +
                  "/*mut267*/[&]{return " + FD->getNameAsString() +
                  "(0);}();\n}";
      }

      Rewrite.ReplaceText(CharSourceRange::getTokenRange(body->getSourceRange()),
                          bodystr);
    }
}
  
void MutatorFrontendAction_267::MutatorASTConsumer_267::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionDecl().bind("Templates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}