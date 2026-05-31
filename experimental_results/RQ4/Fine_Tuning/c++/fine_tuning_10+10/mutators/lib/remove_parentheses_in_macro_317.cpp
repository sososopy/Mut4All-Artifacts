//source file
#include "../include/Remove_Parentheses_In_Macro_317.h"

// ========================================================================================================
#define MUT317_OUTPUT 1

void MutatorFrontendAction_317::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MI = Result.Nodes.getNodeAs<clang::CallExpr>("macroInvocation")) {
      if (!MI || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MI->getExprLoc()))
        return;

      for (unsigned i = 0; i < MI->getNumArgs(); ++i) {
        const Expr *arg = MI->getArg(i);
        if (isa<InitListExpr>(arg)) {
          SourceRange argRange = arg->getSourceRange();
          std::string argText = Lexer::getSourceText(CharSourceRange::getTokenRange(argRange), 
                                                     *Result.SourceManager, 
                                                     Result.Context->getLangOpts()).str();
          std::string newArgText = argText.substr(1, argText.size() - 2); // Remove parentheses
          Rewrite.ReplaceText(argRange, newArgText);
          break;
        }
      }
    }
}
  
void MutatorFrontendAction_317::MutatorASTConsumer_317::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = callExpr(callee(functionDecl(hasName("MACRO_NAME"))), 
                            hasAnyArgument(initListExpr())).bind("macroInvocation");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}