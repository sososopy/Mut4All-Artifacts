//source file
#include "../include/invalid_concept_syntax_mutation_14.h"

// ========================================================================================================
#define MUT14_OUTPUT 1

void MutatorFrontendAction_14::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CD = Result.Nodes.getNodeAs<clang::ConceptDecl>("conceptDecl")) {
      if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CD->getLocation()))
        return;

      if (auto *RT = CD->getConstraintExpr()) {
        auto sourceMgr = Result.SourceManager;
        auto requiresClause = clang::Lexer::getSourceText(
            clang::CharSourceRange::getTokenRange(RT->getSourceRange()), *sourceMgr, Result.Context->getLangOpts());

        std::string mutatedClause = "requires requires " + requiresClause.str();
        Rewrite.ReplaceText(RT->getSourceRange(), mutatedClause);
      }
    }
}
  
void MutatorFrontendAction_14::MutatorASTConsumer_14::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = conceptDecl().bind("conceptDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}