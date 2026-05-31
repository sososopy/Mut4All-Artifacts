//source file
#include "../include/Remove_Concept_In_Function_Template_304.h"

// ========================================================================================================
#define MUT304_OUTPUT 1

void MutatorFrontendAction_304::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FuncTemplates")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;

      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 MT->getSourceRange());
      llvm::outs() << declaration << '\n';
      auto params = MT->getTemplateParameters();
      for (auto param : *params) {
        if (auto typeparam = dyn_cast<TemplateTypeParmDecl>(param)) {
          if (typeparam->hasTypeConstraint()) {
            auto conceptname = typeparam->getTypeConstraint()
                                   ->getNamedConcept()
                                   ->getNameAsString();
            llvm::outs() << conceptname << '\n';
            auto pos = declaration.find(conceptname);
            if (pos != string::npos) {
              declaration.erase(pos, conceptname.length());
            }
          }
        }
      }
      llvm::outs() << declaration << '\n';
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_304::MutatorASTConsumer_304::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl().bind("FuncTemplates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}