//source file
#include "../include/invalid_defaulted_comparison_in_class_326.h"

// ========================================================================================================
#define MUT326_OUTPUT 1

void MutatorFrontendAction_326::Callback::run(const MatchFinder::MatchResult &Result) {
  if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
    if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   CL->getLocation()))
      return;
    if ((!CL->isStruct() && !CL->isClass()) || CL->isLambda())
      return;
    if (!CL->isCompleteDefinition())
      return;
    if (CL->hasDefinition() == false)
      return;
    auto fields = CL->fields();
    if (fields.begin() == fields.end())
      return;
    candidate_classes.push_back(CL);
  } else if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>( "TargetClass")) {
    if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile( CL->getLocation())) return;
    if (CL->hasDefinition() == false) return;
    auto content = stringutils::rangetoStr(*(Result.SourceManager), CL->getSourceRange());
    if (content.find("operator==") != string::npos) {
      llvm::outs() << "mut326: found user-defined operator==\n";
      return;
    }
    auto ins = "/*mut326*/bool operator==(const " + CL->getNameAsString() +
               "&)=default;\n";
    Rewrite.ReplaceText(CL->getEndLoc(), 0, ins);
  }
}

void MutatorFrontendAction_326::MutatorASTConsumer_326::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Classes");
    auto target_matcher = cxxRecordDecl(isSameOrDerivedFrom(hasAnyName(
                                            "std::strong_ordering", "std::weak_ordering",
                                            "std::partial_ordering")))
                              .bind("TargetClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(target_matcher, &callback);
    matchFinder.matchAST(Context);
}