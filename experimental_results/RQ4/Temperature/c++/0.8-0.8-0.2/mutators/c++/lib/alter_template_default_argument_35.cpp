//source file
#include "../include/Mutator_35.h"

// ========================================================================================================
#define MUT35_OUTPUT 1

void MutatorFrontendAction_35::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("TemplateFunction")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
        return;

      const TemplateParameterList *TPList = FD->getTemplateParameters();
      for (unsigned i = 0; i < TPList->size(); ++i) {
        if (auto *TP = dyn_cast<TemplateTypeParmDecl>(TPList->getParam(i))) {
          if (!TP->hasDefaultArgument()) {
            auto nameLoc = TP->getLocation();
            std::string replacement = " = double"; // Inconsistent default argument
            Rewrite.InsertTextAfter(nameLoc, replacement);
            break; // Only mutate the first non-default parameter
          }
        }
      }
    }
}
  
void MutatorFrontendAction_35::MutatorASTConsumer_35::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl().bind("TemplateFunction");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}