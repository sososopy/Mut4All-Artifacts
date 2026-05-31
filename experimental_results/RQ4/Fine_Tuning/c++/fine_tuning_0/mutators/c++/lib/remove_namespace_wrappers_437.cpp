//source file
#include "../include/Remove_Namespace_Wrappers_437.h"

// ========================================================================================================
#define MUT437_OUTPUT 1

void MutatorFrontendAction_437::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ED = Result.Nodes.getNodeAs<clang::EnumDecl>("Enums")) {
      if (!ED || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ED->getLocation()))
        return;
      if (!ED->isCompleteDefinition())
        return;
      if (ED->getAccess() == AccessSpecifier::AS_public)
        return;
      auto parent = ED->getParent();
      if (!llvm::isa<clang::CXXRecordDecl>(parent))
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             ED->getSourceRange());
      auto parent_content = stringutils::rangetoStr(*(Result.SourceManager),
                                                    llvm::cast<clang::CXXRecordDecl>(parent)->getSourceRange());
      llvm::outs() << "enum: " << content << '\n';
      llvm::outs() << "parent: " << parent_content << '\n';
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(ED->getSourceRange()),
                          parent_content);
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(llvm::cast<clang::CXXRecordDecl>(parent)->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_437::MutatorASTConsumer_437::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = enumDecl().bind("Enums");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}