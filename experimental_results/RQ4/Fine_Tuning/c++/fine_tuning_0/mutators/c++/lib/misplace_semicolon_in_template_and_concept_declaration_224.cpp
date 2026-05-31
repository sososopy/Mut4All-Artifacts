//source file
#include "../include/Misplace_Semicolon_In_Template_And_Concept_Declaration_224.h"

// ========================================================================================================
#define MUT224_OUTPUT 1

void MutatorFrontendAction_224::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateDecl>("Templates")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 MT->getSourceRange());
      declaration.insert(declaration.find('<'), ";");
      declaration = "/*mut224*/" + declaration;
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    } else if (auto *CT =
                   Result.Nodes.getNodeAs<clang::ConceptDecl>("Concepts")) {
      if (!CT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CT->getLocation()))
        return;
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 CT->getSourceRange());
      declaration.insert(declaration.find_first_of(" \t"), ";");
      declaration = "/*mut224*/" + declaration;
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(CT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_224::MutatorASTConsumer_224::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = clang::ast_matchers::decl().bind("Templates");
    auto concept_matcher = clang::ast_matchers::decl().bind("Concepts");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(concept_matcher, &callback);
    matchFinder.matchAST(Context);
}