//source file
#include "../include/misuse_template_arguments_in_function_calls_10.h"

// ========================================================================================================
#define MUT10_OUTPUT 1

void MutatorFrontendAction_10::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FC = Result.Nodes.getNodeAs<clang::CallExpr>("FunctionCall")) {
      if (!FC || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FC->getExprLoc()))
        return;

      const FunctionDecl *FD = FC->getDirectCallee();
      if (!FD || !FD->isFunctionTemplateSpecialization())
        return;

      const TemplateArgumentList *TAL = FD->getTemplateSpecializationArgs();
      if (!TAL)
        return;

      std::string mutatedCall;
      if (TAL->size() > 0) {
        mutatedCall = FD->getNameInfo().getAsString() + "<int, double>(";
      } else {
        mutatedCall = FD->getNameInfo().getAsString() + "<int>(";
      }

      for (unsigned i = 0; i < FC->getNumArgs(); ++i) {
        if (i > 0)
          mutatedCall += ", ";
        mutatedCall += Lexer::getSourceText(CharSourceRange::getTokenRange(FC->getArg(i)->getSourceRange()), *Result.SourceManager, Result.Context->getLangOpts()).str();
      }
      mutatedCall += ")";

      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FC->getSourceRange()), mutatedCall);
    }
}
  
void MutatorFrontendAction_10::MutatorASTConsumer_10::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    StatementMatcher matcher = callExpr(callee(functionDecl(isTemplateInstantiation()))).bind("FunctionCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}