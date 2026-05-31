//source file
#include "../include/modify_template_requirements_316.h"

// ========================================================================================================
#define MUT316_OUTPUT 1

void MutatorFrontendAction_316::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CT = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("TemplateConstructor")) {
      if (!CT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CT->getLocation()))
        return;

      if (CT->getNumParams() != 1)
        return;

      auto *T = CT->getTemplateInstantiationPattern();
      if (!T)
        return;

      std::string originalCode = stringutils::rangetoStr(*(Result.SourceManager), CT->getSourceRange());

      size_t requiresPos = originalCode.find("requires requires");
      if (requiresPos != std::string::npos) {
        std::string mutatedCode = originalCode;
        mutatedCode.insert(requiresPos, "requires requires(Y y) { a.x = y; }; ");
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(CT->getSourceRange()), mutatedCode);
      }
    }
}
  
void MutatorFrontendAction_316::MutatorASTConsumer_316::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxConstructorDecl(
        hasParent(cxxRecordDecl(isStruct(), hasDescendant(classTemplateSpecializationDecl(
            hasTemplateArgument(0, templateArgument().bind("TemplateArgument"))))))
    ).bind("TemplateConstructor");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}