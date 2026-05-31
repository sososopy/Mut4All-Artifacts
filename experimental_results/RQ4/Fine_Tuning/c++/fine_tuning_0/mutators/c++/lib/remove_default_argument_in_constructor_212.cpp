//source file
#include "../include/Remove_Default_Argument_In_Constructor_212.h"

// ========================================================================================================
#define MUT212_OUTPUT 1

void MutatorFrontendAction_212::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CD = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("Constructor")) {
      if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CD->getLocation()))
        return;
      if (CD->getNumParams() == 0)
        return;
      if (!CD->getParamDecl(0)->hasDefaultArg())
        return;
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 CD->getSourceRange());
      llvm::outs() << declaration << '\n';
      auto params = CD->parameters();
      for (auto param : params) {
        if (param->hasDefaultArg()) {
          auto param_str = stringutils::rangetoStr(
              *(Result.SourceManager), param->getSourceRange());
          auto equal_pos = param_str.find('=');
          if (equal_pos == string::npos)
            continue;
          param_str = param_str.substr(0, equal_pos + 1);
          declaration.replace(param->getBeginLoc().getRawEncoding() - CD->getBeginLoc().getRawEncoding(),
                              param->getEndLoc().getRawEncoding() - param->getBeginLoc().getRawEncoding() + 1,
                              param_str);
        }
      }
      llvm::outs() << declaration << '\n';
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(CD->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_212::MutatorASTConsumer_212::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxConstructorDecl().bind("Constructor");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}