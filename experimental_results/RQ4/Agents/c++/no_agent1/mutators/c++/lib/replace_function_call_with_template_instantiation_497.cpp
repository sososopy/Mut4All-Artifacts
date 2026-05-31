//source file
#include "../include/replace_function_call_with_template_instantiation_497.h"

// ========================================================================================================
#define MUT497_OUTPUT 1

void MutatorFrontendAction_497::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FC = Result.Nodes.getNodeAs<clang::CallExpr>("FunctionCalls")) {
      if (!FC || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FC->getBeginLoc()))
        return;

      auto callee = FC->getDirectCallee();
      if (!callee || callee->getTemplatedKind() != FunctionDecl::TK_NonTemplate)
        return;

      auto calleeName = callee->getNameAsString();
      std::string templateInstantiation = calleeName + "<int>(";

      for (unsigned i = 0; i < FC->getNumArgs(); ++i) {
        if (i > 0) templateInstantiation += ", ";
        templateInstantiation += Lexer::getSourceText(CharSourceRange::getTokenRange(
                                  FC->getArg(i)->getSourceRange()), 
                                  *Result.SourceManager, 
                                  Result.Context->getLangOpts()).str();
      }

      templateInstantiation += ")";
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FC->getSourceRange()), templateInstantiation);
    }
}
  
void MutatorFrontendAction_497::MutatorASTConsumer_497::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    StatementMatcher matcher = callExpr(callee(functionDecl())).bind("FunctionCalls");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}