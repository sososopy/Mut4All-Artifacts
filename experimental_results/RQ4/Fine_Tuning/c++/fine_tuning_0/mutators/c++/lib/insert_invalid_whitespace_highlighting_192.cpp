//source file
#include "../include/Insert_Invalid_Whitespace_Highlighting_192.h"

// ========================================================================================================
#define MUT192_OUTPUT 1

void MutatorFrontendAction_192::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::DecompositionDecl>("Decompositions")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      auto LHS = DL->bindings();
      auto LHS_type = DL->getType();
      auto LHS_type_str = stringutils::rangetoStr(*(Result.SourceManager),
                                                  DL->getTypeSourceInfo()->getTypeLoc().getSourceRange());
      auto RHS = DL->getInit();
      auto RHS_str = stringutils::rangetoStr(*(Result.SourceManager),
                                             RHS->getSourceRange());
      llvm::outs() << LHS_type_str << '\n';
      llvm::outs() << RHS_str << '\n';
      auto LHS_str = stringutils::rangetoStr(*(Result.SourceManager),
                                             DL->getSourceRange());
      llvm::outs() << LHS_str << '\n';
      auto LHS_str_mut = LHS_str;
      for (auto LHS_var : LHS) {
        auto LHS_var_str = LHS_var->getNameAsString();
        auto pos = LHS_str_mut.find(LHS_var_str);
        if (pos == string::npos)
          continue;
        LHS_str_mut.insert(pos, " ");
        LHS_str_mut.insert(pos + 1 + LHS_var_str.size(), " ");
      }
      llvm::outs() << LHS_str_mut << '\n';
      auto mut =
          "/*mut192*/" + LHS_type_str + LHS_str_mut + "=" + " " + RHS_str;
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()), mut);
    }
}
  
void MutatorFrontendAction_192::MutatorASTConsumer_192::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = decompositionDecl().bind("Decompositions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}