//source file
#include "../include/Modify_Concept_Requirement_323.h"

// ========================================================================================================
#define MUT323_OUTPUT 1

void MutatorFrontendAction_323::Callback::run(const MatchFinder::MatchResult &Result) {
    if (const auto *ReqExpr = Result.Nodes.getNodeAs<Expr>("ConceptExpr")) {
      if (!ReqExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ReqExpr->getBeginLoc()))
        return;

      auto sourceRange = ReqExpr->getSourceRange();
      std::string sourceText = Lexer::getSourceText(CharSourceRange::getTokenRange(sourceRange), *Result.SourceManager, Result.Context->getLangOpts()).str();

      // Perform mutation on the source code text by applying string replacement
      size_t autoPos = sourceText.find("auto");
      if (autoPos != std::string::npos) {
        sourceText.replace(autoPos, 4, "int");
      }

      // Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), sourceText);
    }
}
  
void MutatorFrontendAction_323::MutatorASTConsumer_323::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation.
    auto matcher = expr(hasDescendant(declRefExpr(to(varDecl(hasType(autoType())))))).bind("ConceptExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}