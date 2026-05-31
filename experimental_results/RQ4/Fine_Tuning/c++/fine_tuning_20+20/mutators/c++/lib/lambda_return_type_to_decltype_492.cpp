//source file
#include "../include/Lambda_Return_Type_To_Decltype_492.h"

// ========================================================================================================
#define MUT492_OUTPUT 1

void MutatorFrontendAction_492::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *DL = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambdas")) {
      //Filter nodes in header files
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto lambda =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      if (DL->hasExplicitParameters() && DL->hasExplicitResultType()) {
        llvm::outs() << "Mut492: explicit both\n";
        llvm::outs() << lambda << '\n';
        size_t pos = lambda.rfind("auto");
        if (pos != string::npos) {
          lambda.replace(pos, 4, "decltype(auto)");
        }
      } else if (DL->hasExplicitParameters()) {
        llvm::outs() << "Mut492: explicit param\n";
        llvm::outs() << lambda << '\n';
        size_t pos = lambda.rfind(')');
        if (pos != string::npos) {
          lambda.insert(pos + 1, " -> decltype(auto) ");
        }
      } else {
        llvm::outs() << "Mut492: implicit both\n";
        llvm::outs() << lambda << '\n';
        size_t pos = lambda.find('[');
        if (pos != string::npos) {
          lambda.insert(pos, "decltype(auto) ");
        }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()), lambda);
    }
}
  
void MutatorFrontendAction_492::MutatorASTConsumer_492::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = lambdaExpr().bind("Lambdas");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}