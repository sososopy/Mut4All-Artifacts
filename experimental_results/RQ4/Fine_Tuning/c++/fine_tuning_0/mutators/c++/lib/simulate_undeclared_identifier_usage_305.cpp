//source file
#include "../include/simulate_undeclared_identifier_usage_305.h"

// ========================================================================================================
#define MUT305_OUTPUT 1

void MutatorFrontendAction_305::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (!FD->hasBody())
        return;
      auto body = FD->getBody();
      auto funcname = FD->getNameAsString();
      llvm::outs() << funcname << '\n';
      auto typo = funcname + funcname;
      auto ifstmt = "if constexpr(" + typo + "<>){}";
      ifstmt = "/*mut305*/" + ifstmt;
      Rewrite.ReplaceText(body->getBeginLoc(), 0, ifstmt);
    }
}

void MutatorFrontendAction_305::MutatorASTConsumer_305::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionDecl().bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}