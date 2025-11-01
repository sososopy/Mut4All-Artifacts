//source file
#include "../include/Mutator_155.h"

// ========================================================================================================
#define MUT155_OUTPUT 1

void MutatorFrontendAction_155::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("MemberFunction")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;

      if (const auto *Parent = dyn_cast<CXXRecordDecl>(MT->getParent())) {
        if (Parent->isCompleteDefinition()) {
          std::string className = Parent->getNameAsString();
          std::string methodName = MT->getNameAsString();
          std::string pointerToMember = "void (" + className + "::*)()";
          std::string nestedSpecifier = className + "::" + methodName;

          std::string mutationText = "/*mut155*/" + nestedSpecifier + " = &" + className + "::" + methodName + ";";
          SourceLocation insertLoc = MT->getBeginLoc();
          Rewrite.InsertTextBefore(insertLoc, mutationText);
        }
      }
    }
}
  
void MutatorFrontendAction_155::MutatorASTConsumer_155::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxMethodDecl().bind("MemberFunction");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}