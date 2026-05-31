//source file
#include "../include/add_recursive_field_in_class_6.h"

// ========================================================================================================
#define MUT6_OUTPUT 1

void MutatorFrontendAction_6::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CL->getLocation()))
        return;
      if ((!CL->isStruct() && !CL->isClass()) || CL->isLambda())
        return;
      if (!CL->isCompleteDefinition())
        return;

      auto className = CL->getNameAsString();
      if (className.empty())
        return;

      auto content = stringutils::rangetoStr(*(Result.SourceManager), CL->getSourceRange());
      std::string recursiveField = "\n\t/*mut6*/" + className + " selfReference;\n";
      std::string memberFunction = "\n\tvoid dummyFunction() {}\n";

      if (content.rfind('}') != std::string::npos) {
        content.insert(content.rfind('}'), recursiveField + memberFunction);
      }

      Rewrite.ReplaceText(CharSourceRange::getTokenRange(CL->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_6::MutatorASTConsumer_6::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}