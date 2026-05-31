//source file
#include "../include/Invalid_Access_Specifier_327.h"

// ========================================================================================================
#define MUT327_OUTPUT 1

void MutatorFrontendAction_327::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CL->getLocation()))
        return;
      if ((!CL->isStruct() && !CL->isClass()) || CL->isLambda())
        return;
      if (!CL->isCompleteDefinition())
        return;
      cur_classes.push_back(CL);
    } else if (auto *AS =
                   Result.Nodes.getNodeAs<clang::AccessSpecDecl>("Access")) {
      if (!AS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     AS->getLocation()))
        return;
      // llvm::outs()<<"mut327: "<<AS->getAccess()<<'\n';
      if (AS->getAccess() == AccessSpecifier::AS_none)
        return;
      Rewrite.ReplaceText(AS->getBeginLoc(), 0, "/*mut327*/");
      Rewrite.ReplaceText(AS->getEndLoc(), 0, "internal");
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>(
                   "NoAccessClass")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      for (auto cur_class : cur_classes) {
        if (DL == cur_class)
          return;
      }
      Rewrite.ReplaceText(DL->getBeginLoc(), 0, "/*mut327*/:");
    }
}
  
void MutatorFrontendAction_327::MutatorASTConsumer_327::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Classes");
    auto access_matcher = accessSpecDecl().bind("Access");
    auto noaccess_matcher =
        cxxRecordDecl(unless(has(accessSpecDecl()))).bind("NoAccessClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(access_matcher, &callback);
    matchFinder.addMatcher(noaccess_matcher, &callback);
    matchFinder.matchAST(Context);
}