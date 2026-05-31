//source file
#include "../include/Modify_Constant_Evaluation_Context_419.h"

// ========================================================================================================
#define MUT419_OUTPUT 1

void MutatorFrontendAction_419::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("ConstVar")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (!DL->isConstexpr())
        return;
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      auto type = DL->getType().getAsString();
      auto name = DL->getNameAsString();
      auto init = DL->getInit();
      if (init == nullptr)
        return;
      auto init_str = stringutils::rangetoStr(*(Result.SourceManager),
                                              init->getSourceRange());
      auto mutated_init = "(" + init_str + ">0)?" + init_str +
                          ": (std::cout<<\"not constexpr\", 0)";
      auto mutated_decl = "/*mut419*/" + type + " " + name + "=" + mutated_init;
      llvm::outs() << mutated_decl << '\n';
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()),
                          mutated_decl);
    } else if (auto *DL =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("ConstFunc")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (!DL->isConstexpr())
        return;
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      llvm::outs() << content << '\n';
      auto body = DL->getBody();
      if (body == nullptr)
        return;
      auto body_str = stringutils::rangetoStr(*(Result.SourceManager),
                                              body->getSourceRange());
      auto mutated_body = "{if constexpr (false) std::cout<<\"not constexpr\";"
                          "else"
                          " " +
                          body_str + "}";
      llvm::outs() << mutated_body << '\n';
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(body->getSourceRange()),
                          mutated_body);
    }
}
  
void MutatorFrontendAction_419::MutatorASTConsumer_419::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = varDecl().bind("ConstVar");
    auto matcher2 = functionDecl().bind("ConstFunc");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(matcher2, &callback);
    matchFinder.matchAST(Context);
}