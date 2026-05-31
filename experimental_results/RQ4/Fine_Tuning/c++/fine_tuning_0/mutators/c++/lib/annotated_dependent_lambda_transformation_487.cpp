//source file
#include "../include/Annotated_Dependent_Lambda_Transformation_487.h"

// ========================================================================================================
#define MUT487_OUTPUT 1

void MutatorFrontendAction_487::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::LambdaExpr>("DependentLambdas")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getBeginLoc()))
        return;
      auto DL_str = stringutils::rangetoStr(*(Result.SourceManager),
                                            DL->getSourceRange());
      llvm::outs() << DL_str << '\n';
      if (DL->getExplicitTemplateParameters().empty() == false) {
        llvm::outs() << "has explicit template args\n";
        return;
      }
      if (DL->hasExplicitParameters() == false) {
        DL_str.insert(1, "=");
        Rewrite.ReplaceText(DL->getSourceRange(), DL_str);
        return;
      }
      DL_str.insert(1, "=");
      Rewrite.ReplaceText(DL->getSourceRange(), DL_str);
    }
}
  
void MutatorFrontendAction_487::MutatorASTConsumer_487::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = lambdaExpr(hasAncestor(functionTemplateDecl())).bind("DependentLambdas");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}