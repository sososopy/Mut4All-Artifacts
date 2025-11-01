//source file
#include "../include/Mutator_434.h"

// ========================================================================================================
#define MUT434_OUTPUT 1

void MutatorFrontendAction_434::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *SD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("StructDecl")) {
      if (!SD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     SD->getLocation()))
        return;

      if (SD->hasAttrs()) {
        auto &SM = *Result.SourceManager;
        auto startLoc = SD->getBeginLoc();
        auto endLoc = SD->getLocation();
        // Remove attributes from the struct
        for (const auto *Attr : SD->attrs()) {
          auto attrRange = Attr->getRange();
          Rewrite.RemoveText(attrRange);
        }
        // Ensure struct remains valid
        if (startLoc.isValid() && endLoc.isValid()) {
          auto structText = stringutils::rangetoStr(SM, SourceRange(startLoc, endLoc));
          Rewrite.ReplaceText(SourceRange(startLoc, endLoc), structText);
        }
      }
    }
}
  
void MutatorFrontendAction_434::MutatorASTConsumer_434::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(isStruct()).bind("StructDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}