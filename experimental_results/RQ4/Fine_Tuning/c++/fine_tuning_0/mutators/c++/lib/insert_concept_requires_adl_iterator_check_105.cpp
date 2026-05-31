//source file
#include "../include/Insert_Concept_Requires_ADL_Iterator_Check_105.h"

// ========================================================================================================
#define MUT105_OUTPUT 1

void MutatorFrontendAction_105::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CT = Result.Nodes.getNodeAs<clang::ConceptDecl>("Concepts")) {
      if (!CT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CT->getLocation()))
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             CT->getSourceRange());
      llvm::outs() << content << '\n';
      content.insert(content.find('{') + 1,
                     "/*mut105*/[&]{using std::begin;using std::end;using "
                     "std::swap;return requires{typename "
                     "std::input_iterator<T>;};}()");
      llvm::outs() << content << '\n';
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(CT->getSourceRange()),
                          content);
    }
}
  
void MutatorFrontendAction_105::MutatorASTConsumer_105::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = clang::ast_matchers::decl().bind("Concepts");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}