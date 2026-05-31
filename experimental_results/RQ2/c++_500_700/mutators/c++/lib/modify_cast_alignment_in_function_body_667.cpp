//source file
#include "../include/modify_cast_alignment_in_function_body_667.h"

// ========================================================================================================
#define MUT667_OUTPUT 1

void MutatorFrontendAction_667::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CE = Result.Nodes.getNodeAs<clang::CStyleCastExpr>("CastExpr")) {
      if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CE->getBeginLoc()))
        return;

      auto typeStr = CE->getTypeAsWritten().getAsString();
      std::string newTypeStr = "alignas(8) " + typeStr;
      
      auto range = CharSourceRange::getTokenRange(CE->getTypeInfoAsWritten()->getTypeLoc().getSourceRange());
      Rewrite.ReplaceText(range, newTypeStr);
    }
}
  
void MutatorFrontendAction_667::MutatorASTConsumer_667::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cStyleCastExpr(hasType(pointerType())).bind("CastExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}