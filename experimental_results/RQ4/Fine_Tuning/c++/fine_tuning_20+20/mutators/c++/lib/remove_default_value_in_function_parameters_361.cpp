//source file
#include "../include/remove_default_value_in_function_parameters_361.h"

// ========================================================================================================
#define MUT361_OUTPUT 1

void MutatorFrontendAction_361::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getNumParams() == 0)
        return;
      cur_functions.push_back(FD);
    } else if (auto *PD = Result.Nodes.getNodeAs<clang::ParmVarDecl>( "Params")) {
      if (!PD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     PD->getLocation()))
        return;
      if (!PD->hasDefaultArg())
        return;
      auto param_text = stringutils::rangetoStr(*(Result.SourceManager),
                                                PD->getSourceRange());
      llvm::outs() << param_text << '\n';
      if (param_text.find('=') != string::npos) {
        param_text.erase(param_text.find('='));
        llvm::outs() << param_text << '\n';
      }
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(PD->getSourceRange()),
                          param_text);
    }
}
  
void MutatorFrontendAction_361::MutatorASTConsumer_361::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionDecl().bind("Functions");
    auto param_matcher = parmVarDecl(hasAncestor(functionDecl())).bind("Params");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(param_matcher, &callback);
    matchFinder.matchAST(Context);
}