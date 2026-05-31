//source file
#include "../include/Template_Constraint_Circular_Dependency_481.h"

// ========================================================================================================
#define MUT481_OUTPUT 1

void MutatorFrontendAction_481::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("TemplateFunctions")) {
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;
      if (TD->isMemberSpecialization())
        return;
      if (!TD->isThisDeclarationADefinition())
        return;
      if (!TD->hasAssociatedConstraints())
        return;
      llvm::outs() << TD->getNameAsString() << '\n';
      llvm::SmallVector<const clang::Expr *> constraints;
      TD->getAssociatedConstraints(constraints);
      if (constraints.empty())
        return;
      auto constraint_expr = stringutils::rangetoStr(
          *(Result.SourceManager),
          clang::SourceRange(constraints.front()->getBeginLoc(),
                             constraints.back()->getEndLoc()));
      llvm::outs() << constraint_expr << '\n';
      auto new_constraint_expr =
          "requires std::is_constructible_v<std::decay_t<T>, T>";
      auto new_constraint = constraint_expr.replace(
          constraint_expr.find("requires"), strlen(new_constraint_expr),
          new_constraint_expr);
      llvm::outs() << new_constraint << '\n';
      Rewrite.ReplaceText(
          clang::CharSourceRange::getTokenRange(
              clang::SourceRange(constraints.front()->getBeginLoc(),
                                 constraints.back()->getEndLoc())),
          new_constraint);
    }
}
  
void MutatorFrontendAction_481::MutatorASTConsumer_481::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl().bind("TemplateFunctions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}