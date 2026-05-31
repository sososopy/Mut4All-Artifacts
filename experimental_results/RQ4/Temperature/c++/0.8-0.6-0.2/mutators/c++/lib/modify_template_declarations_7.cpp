//source file
#include "../include/modify_template_declarations_7.h"

// ========================================================================================================
#define MUT7_OUTPUT 1

void MutatorFrontendAction_7::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateDecl")) {
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;

      auto *TemplatedDecl = TD->getTemplatedDecl();
      if (auto *CXXRD = dyn_cast<CXXRecordDecl>(TemplatedDecl)) {
        if (CXXRD->isCompleteDefinition()) {
          std::string className = CXXRD->getNameAsString();
          std::string mutation = "typedef typename T::type NestedType; void function(NestedType);";
          Rewrite.InsertTextAfterToken(CXXRD->getBraceRange().getBegin(), "/*mut7*/" + mutation);
        }
      }
    }
}
  
void MutatorFrontendAction_7::MutatorASTConsumer_7::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl().bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}