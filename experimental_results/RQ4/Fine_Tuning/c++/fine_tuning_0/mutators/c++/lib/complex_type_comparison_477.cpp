//source file
#include "../include/Complex_Type_Comparison_477.h"

// ========================================================================================================
#define MUT477_OUTPUT 1

void MutatorFrontendAction_477::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("ComplexVar")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->getType()->isComplexType() == false)
        return;
      // llvm::outs() << DL->getNameAsString() << '\n';
      if (cur_func == nullptr)
        return;
      auto real = "__real__ " + DL->getNameAsString();
      auto imag = "__imag__ " + DL->getNameAsString();
      auto real_cmp = real + "<" + "&" + real;
      auto imag_cmp = imag + "<" + "&" + imag;
      auto real_cmp_stmt = "f(" + real_cmp + ");\n";
      auto imag_cmp_stmt = "f(" + imag_cmp + ");\n";
      auto body = stringutils::rangetoStr(*(Result.SourceManager),
                                          cur_func->getBody()->getSourceRange());
      body.insert(1, real_cmp_stmt);
      body.insert(1, imag_cmp_stmt);
      llvm::outs() << body << '\n';
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(cur_func->getBody()->getSourceRange()),
          body);
    } else if (auto *FL =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
      if (!FL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FL->getLocation()))
        return;
      if (FL->hasBody() == false)
        return;
      cur_func = FL;
    }
  }
  
void MutatorFrontendAction_477::MutatorASTConsumer_477::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = varDecl().bind("ComplexVar");
    auto func_matcher = functionDecl().bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(func_matcher, &callback);
    matchFinder.matchAST(Context);
}