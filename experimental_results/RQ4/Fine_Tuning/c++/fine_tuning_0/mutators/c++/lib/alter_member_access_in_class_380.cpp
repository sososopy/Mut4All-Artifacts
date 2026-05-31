//source file
#include "../include/Alter_Member_Access_In_Class_380.h"

// ========================================================================================================
#define MUT380_OUTPUT 1

void MutatorFrontendAction_380::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CL->getLocation()))
        return;
      if ((!CL->isStruct() && !CL->isClass()) || CL->isLambda())
        return;
      cur_classes.push_back(CL);
    } else if (auto *MA = Result.Nodes.getNodeAs<clang::MemberExpr>("Members")) {
      if (!MA || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MA->getBeginLoc()))
        return;
      llvm::outs() << "mut380: member access found\n";
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), MA->getSourceRange());
      content += "mut380";
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MA->getSourceRange()), content);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>(
                   "Derived")) { // If there is no member access in the class, insert one
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      auto decls = DL->decls();
      bool flag = false;
      for (auto decl : decls) {
        if (decl->getIdentifierNamespace() ==
            Decl::IdentifierNamespace::IDNS_Using) {
          flag = true;
        }
      }
      if (flag == true)
        return;
      for (std::size_t j = 0; j < cur_classes.size(); ++j) {
        if (DL->isDerivedFrom(cur_classes[j])) {
          llvm::outs() << "mut380: derived class found\n";
          auto name = DL->getNameAsString();
          string ins = name + " mut380; mut380.mut380;";
          Rewrite.ReplaceText(DL->getEndLoc(), 0, ins);
        }
      }
    }
  }
  
void MutatorFrontendAction_380::MutatorASTConsumer_380::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Classes");
    auto member_matcher = memberExpr().bind("Members");
    auto derived_matcher =
        cxxRecordDecl(hasAnyBase(hasType(cxxRecordDecl()))).bind("Derived");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(member_matcher, &callback);
    matchFinder.addMatcher(derived_matcher, &callback);
    matchFinder.matchAST(Context);
}