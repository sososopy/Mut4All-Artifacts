//source file
#include "../include/Mutate_Doxygen_Comments_In_Classes_342.h"

// ========================================================================================================
#define MUT342_OUTPUT 1

void MutatorFrontendAction_342::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CL->getLocation()))
        return;
      if ((!CL->isStruct() && !CL->isClass()) || CL->isLambda())
        return;
      if (!CL->isCompleteDefinition())
        return;
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), CL->getSourceRange());
      auto pos = content.find("@param");
      if (pos != string::npos) {
        content.replace(pos, 6, "Satisfies the [`Add<@doc.self>`]($sus::num::Add) concept.");
      }
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(CL->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_342::MutatorASTConsumer_342::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}