//source file
#include "../include/Incomplete_Struct_With_Constexpr_5.h"

// ========================================================================================================
#define MUT5_OUTPUT 1

void MutatorFrontendAction_5::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *SD = Result.Nodes.getNodeAs<clang::RecordDecl>("incompleteStruct")) {
      if (!SD || !Result.Context->getSourceManager().isWrittenInMainFile(SD->getLocation()))
        return;

      if (!SD->isCompleteDefinition()) {
        std::string structName = SD->getNameAsString();
        std::string constexprDecl = "constexpr struct " + structName + " myVariable = { 0 }; /*mut5*/";
        SourceLocation insertLoc = SD->getEndLoc().getLocWithOffset(1);
        Rewrite.InsertText(insertLoc, "\n" + constexprDecl);
      }
    }
}
  
void MutatorFrontendAction_5::MutatorASTConsumer_5::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = recordDecl(unless(isDefinition())).bind("incompleteStruct");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}