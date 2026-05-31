//source file
#include "../include/replace_member_with_incomplete_type_6.h"

// ========================================================================================================
#define MUT6_OUTPUT 1

void MutatorFrontendAction_6::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FieldDecl>("Field")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      
      const auto *ParentRecord = llvm::dyn_cast<clang::CXXRecordDecl>(FD->getParent());

      if (!ParentRecord || !ParentRecord->isCompleteDefinition())
        return;

      std::string ParentName = ParentRecord->getNameAsString();
      std::string FieldDeclText = FD->getNameAsString();
      std::string MutatedText = ParentName + " " + FieldDeclText + ";";
      
      Rewrite.ReplaceText(FD->getSourceRange(), MutatedText);
    }
}
  
void MutatorFrontendAction_6::MutatorASTConsumer_6::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = fieldDecl(hasParent(cxxRecordDecl())).bind("Field");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}