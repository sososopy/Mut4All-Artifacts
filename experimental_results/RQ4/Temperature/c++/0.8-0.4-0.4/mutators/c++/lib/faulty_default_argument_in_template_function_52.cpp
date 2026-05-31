//source file
#include "../include/Mutator_52.h"

// ========================================================================================================
#define MUT52_OUTPUT 1

void MutatorFrontendAction_52::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionWithDefaultArg")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->getNumParams() > 0) {
        const ParmVarDecl *param = FD->getParamDecl(0);
        if (param->hasDefaultArg()) {
          auto defaultArgExpr = param->getDefaultArg();
          if (defaultArgExpr) {
            std::string mutatedArg = "calculate(innerFunc(num)())";
            Rewrite.ReplaceText(defaultArgExpr->getSourceRange(), mutatedArg);
          }
        }
      }
    }
}
  
void MutatorFrontendAction_52::MutatorASTConsumer_52::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(has(compoundStmt()), hasAncestor(classTemplateSpecializationDecl())).bind("FunctionWithDefaultArg");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}