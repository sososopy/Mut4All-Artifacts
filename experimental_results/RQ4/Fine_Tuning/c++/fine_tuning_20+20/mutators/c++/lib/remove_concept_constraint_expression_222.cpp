//source file
#include "../include/remove_concept_constraint_expression_222.h"

// ========================================================================================================
#define MUT222_OUTPUT 1

void MutatorFrontendAction_222::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CD = Result.Nodes.getNodeAs<clang::ConceptDecl>("Concepts")) {
      if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CD->getLocation()))
        return;
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), CD->getSourceRange());
      if (CD->getConstraintExpr() != nullptr) {
        llvm::outs() << content << '\n';
        content = content.substr(0, content.find('=') + 1) + ";";
        llvm::outs() << content << '\n';
      }
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(CD->getSourceRange()),
                          content);
    }
}

void MutatorFrontendAction_222::MutatorASTConsumer_222::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = clang::ast_matchers::decl().bind("Concepts");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}