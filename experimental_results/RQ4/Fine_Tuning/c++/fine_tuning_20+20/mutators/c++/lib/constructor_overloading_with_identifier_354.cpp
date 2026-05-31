//source file
#include "../include/constructor_overloading_with_identifier_354.h"

// ========================================================================================================
#define MUT354_OUTPUT 1

void MutatorFrontendAction_354::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
    if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   CL->getLocation()))
      return;
    if (CL->isCompleteDefinition() == false)
      return;
    cur_classes.push_back(CL);
  } else if (auto *CD = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>( "Constructors")) {
    if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(
                   CD->getLocation()))
      return;
    if (!CD->isUserProvided())
      return;
    if (CD->isExplicit())
      return;
    if (CD->isDeleted())
      return;
    if (CD->isCopyOrMoveConstructor())
      return;
    if (CD->getNumParams() != 0)
      return;
    if (CD->isInheritingConstructor())
      return;
    if (CD->getAccess() == AccessSpecifier::AS_protected)
      return;
    if (CD->getAccess() == AccessSpecifier::AS_private)
      return;
    auto record = CD->getParent();
    if (record->isLambda())
      return;
    auto recordname = record->getNameAsString();
    if (recordname == "")
      return;
    auto content =
        stringutils::rangetoStr(*(Result.SourceManager), CD->getSourceRange());

    if (content.find(':') != string::npos)
      return;
    if (content.find("mut_354") != string::npos)
      return;
    content.insert(content.find('{'), "int mut_354 ");
    Rewrite.ReplaceText(CharSourceRange::getTokenRange(CD->getSourceRange()),
                        content);
  } else if (auto *SA = Result.Nodes.getNodeAs<clang::StaticAssertDecl>(
                 "StaticAsserts")) {
    if (!SA || !Result.Context->getSourceManager().isWrittenInMainFile(
                   SA->getLocation()))
      return;
    if (SA->isFailed())
      return;
    auto content =
        stringutils::rangetoStr(*(Result.SourceManager), SA->getSourceRange());
    content = "/*mut354*/" + content;
    Rewrite.ReplaceText(CharSourceRange::getTokenRange(SA->getSourceRange()),
                        content);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>(
                 "DerivedClasses")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isCompleteDefinition() == false)
      return;
    auto decls = DL->decls();
    for (auto decl : decls) {
      if (decl->getIdentifierNamespace() ==
          Decl::IdentifierNamespace::IDNS_Using) {
        return;
      }
    }
    for (std::size_t j = 0; j < cur_classes.size(); ++j) {
      if (DL->isDerivedFrom(cur_classes[j])) {
        auto classname = cur_classes[j]->getNameAsString();
        llvm::outs() << classname << '\n';
        auto ins = "/*mut354*/using " + classname + " :: mut_354;\n";
        Rewrite.ReplaceText(DL->getEndLoc(), 0, ins);
      }
    }
  }
}

void MutatorFrontendAction_354::MutatorASTConsumer_354::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Classes");
  auto constructor_matcher = cxxConstructorDecl().bind("Constructors");
  auto staticassert_matcher = staticAssertDecl().bind("StaticAsserts");
  auto derived_matcher = cxxRecordDecl(hasAnyBase(hasType(cxxRecordDecl()))).bind("DerivedClasses");
  Callback callback(TheRewriter);
  matchFinder.addMatcher(matcher, &callback);
  matchFinder.addMatcher(constructor_matcher, &callback);
  matchFinder.addMatcher(staticassert_matcher, &callback);
  matchFinder.addMatcher(derived_matcher, &callback);
  matchFinder.matchAST(Context);
}