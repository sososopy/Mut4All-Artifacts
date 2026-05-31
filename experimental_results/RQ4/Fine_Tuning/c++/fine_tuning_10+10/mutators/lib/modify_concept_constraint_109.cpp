//source file
#include "../include/modify_concept_constraint_109.h"

using namespace clang;
using namespace clang::ast_matchers;

// ========================================================================================================
#define MUT109_OUTPUT 1

void MutatorFrontendAction_109::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CT = Result.Nodes.getNodeAs<ConceptSpecializationExpr>("ConceptExpr")) {
      if (!CT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CT->getBeginLoc()))
        return;

      auto sourceRange = CT->getSourceRange();
      auto originalText = Lexer::getSourceText(CharSourceRange::getTokenRange(sourceRange), 
                                               *Result.SourceManager, 
                                               Result.Context->getLangOpts()).str();

      std::string mutatedText = originalText;
      size_t pos = mutatedText.find("+");
      if (pos != std::string::npos) {
          mutatedText.replace(pos, 1, "/");
      }

      Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), mutatedText);
    }
}
  
void MutatorFrontendAction_109::MutatorASTConsumer_109::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = expr(hasType(qualType(hasDeclaration(recordDecl(hasName("ConceptSpecializationExpr")))))).bind("ConceptExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}