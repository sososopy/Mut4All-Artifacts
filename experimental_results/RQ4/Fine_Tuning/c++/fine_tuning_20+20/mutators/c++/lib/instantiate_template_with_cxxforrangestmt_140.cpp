//source file
#include "../include/instantiate_template_with_cxxforrangestmt_140.h"

// ========================================================================================================
#define MUT140_OUTPUT 1

void MutatorFrontendAction_140::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FuncTemplate")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FT->getLocation()))
        return;
      if (FT->isThisDeclarationADefinition() == false)
        return;
      // if (FT->getRequiresClause() == nullptr)
      //   return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             FT->getSourceRange());
      if (content.find("for") == string::npos)
        return;
      llvm::outs() << content;
      target = FT;
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::DeclRefExpr>(
                   "DeclRefExpr")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getBeginLoc()))
        return;
      if (target == nullptr)
        return;
      auto target_name = target->getNameAsString();
      if (DL->getNameInfo().getAsString() != target_name)
        return;
      llvm::outs() << DL->getNameInfo().getAsString();
      auto ins = DL->getNameInfo().getAsString() + "<int>()";
      Rewrite.ReplaceText(DL->getSourceRange(), ins);
    }
}
  
void MutatorFrontendAction_140::MutatorASTConsumer_140::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionTemplateDecl().bind("FuncTemplate");
    auto declref_matcher = declRefExpr().bind("DeclRefExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(declref_matcher, &callback);
    matchFinder.matchAST(Context);
}