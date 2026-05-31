//source file
#include "../include/template_deduction_guide_with_scope_change_12.h"

// ========================================================================================================
#define MUT12_OUTPUT 1

void MutatorFrontendAction_12::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *deductionGuide = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("DeductionGuide")) {
      if (!deductionGuide || !Result.Context->getSourceManager().isWrittenInMainFile(
                     deductionGuide->getLocation()))
        return;

      // Get the parent namespace of the template deduction guide
      if (const auto *parentNamespace = Result.Nodes.getNodeAs<NamespaceDecl>("ParentNamespace")) {
        std::string qualifiedTemplateName = parentNamespace->getNameAsString() + "::" + deductionGuide->getTemplatedDecl()->getNameAsString();
        
        // Get the source code of the deduction guide
        auto deductionGuideSource = stringutils::rangetoStr(*(Result.SourceManager), deductionGuide->getSourceRange());

        // Modify the deduction guide to include the namespace qualifier
        std::string newDeductionGuide = deductionGuideSource;
        size_t pos = newDeductionGuide.find(deductionGuide->getTemplatedDecl()->getNameAsString() + "()");
        if (pos != std::string::npos) {
          newDeductionGuide.replace(pos, deductionGuide->getTemplatedDecl()->getNameAsString().length(), qualifiedTemplateName);
        }
        
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(deductionGuide->getSourceRange()), newDeductionGuide);
      }
    }
}
  
void MutatorFrontendAction_12::MutatorASTConsumer_12::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Matcher to identify template deduction guides within a namespace
    auto deductionGuideMatcher = functionTemplateDecl(hasAncestor(namespaceDecl().bind("ParentNamespace"))).bind("DeductionGuide");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(deductionGuideMatcher, &callback);
    matchFinder.matchAST(Context);
}