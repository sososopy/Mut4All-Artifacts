//source file
#include "../include/Convert_Enum_To_Enum_Class_231.h"

// ========================================================================================================
#define MUT231_OUTPUT 1

void MutatorFrontendAction_231::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ED = Result.Nodes.getNodeAs<clang::EnumDecl>("EnumDecl")) {
      if (!ED || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ED->getLocation()))
        return;
      if (ED->isScoped())
        return;
      auto enum_decl = stringutils::rangetoStr(*(Result.SourceManager),
                                               ED->getSourceRange());
      enum_decl.insert(4, " class");
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(ED->getSourceRange()), enum_decl);
    }
}
  
void MutatorFrontendAction_231::MutatorASTConsumer_231::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = enumDecl().bind("EnumDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}