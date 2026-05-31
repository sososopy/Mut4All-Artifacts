//source file
#include "../include/incomplete_static_assert_expression_13.h"

// ========================================================================================================
#define MUT13_OUTPUT 1

void MutatorFrontendAction_13::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *SA = Result.Nodes.getNodeAs<clang::StaticAssertDecl>("StaticAssert")) {
      if (!SA || !Result.Context->getSourceManager().isWrittenInMainFile(
                     SA->getLocation()))
        return;

      auto messageExpr = SA->getMessage();
      if (auto *SL = llvm::dyn_cast<clang::StringLiteral>(messageExpr)) {
        if (SL->getEndLoc().isValid()) {
          auto range = SL->getSourceRange();
          std::string text = Lexer::getSourceText(CharSourceRange::getTokenRange(range), 
                                                  Result.Context->getSourceManager(), 
                                                  Result.Context->getLangOpts()).str();

          if (!text.empty() && text.back() == '"') {
            text.pop_back(); // remove the closing quotation mark
            text += " /*mut13*/"; // add mutation marker
            Rewrite.ReplaceText(range, text);
          }
        }
      }
    }
}
  
void MutatorFrontendAction_13::MutatorASTConsumer_13::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = staticAssertDecl().bind("StaticAssert");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}