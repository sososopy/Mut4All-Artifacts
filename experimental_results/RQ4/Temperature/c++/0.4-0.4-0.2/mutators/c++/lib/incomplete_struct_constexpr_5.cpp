//source file
#include "../include/Incomplete_Struct_Constexpr_5.h"

// ========================================================================================================
#define MUT5_OUTPUT 1

void MutatorFrontendAction_5::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *SD = Result.Nodes.getNodeAs<clang::RecordDecl>("StructDecl")) {
      if (!SD || !Result.Context->getSourceManager().isWrittenInMainFile(SD->getLocation()))
        return;
      if (SD->isCompleteDefinition())
        return;

      SourceLocation insertLoc = SD->getEndLoc().getLocWithOffset(1);
      std::string constexprDecl = "constexpr struct " + SD->getNameAsString() + " y = { 1, 2, 3 };";
      Rewrite.InsertText(insertLoc, "\n" + constexprDecl, true, true);
    }
}

void MutatorFrontendAction_5::MutatorASTConsumer_5::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = recordDecl(unless(isDefinition())).bind("StructDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}