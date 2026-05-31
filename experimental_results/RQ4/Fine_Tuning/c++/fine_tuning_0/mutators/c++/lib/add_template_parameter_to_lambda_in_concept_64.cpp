//source file
#include "../include/Add_Template_Parameter_To_Lambda_In_Concept_64.h"

// ========================================================================================================
#define MUT64_OUTPUT 1

void MutatorFrontendAction_64::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambda")) {
      if (!LE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LE->getBeginLoc()))
        return;

      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             LE->getSourceRange());
      llvm::outs() << content << '\n';
      auto pos = content.find('[');
      if (pos != string::npos) {
        content.insert(pos + 1, "/*mut64*/<typename> ");
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(LE->getSourceRange()),
                            content);
      }
    }
}
  
void MutatorFrontendAction_64::MutatorASTConsumer_64::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher =
        lambdaExpr(hasAncestor(declRefExpr(to(decl())))).bind("Lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}