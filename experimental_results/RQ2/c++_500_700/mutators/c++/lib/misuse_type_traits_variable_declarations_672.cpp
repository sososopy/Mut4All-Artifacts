//source file
#include "../include/misuse_type_traits_variable_declarations_672.h"

// ========================================================================================================
#define MUT672_OUTPUT 1

void MutatorFrontendAction_672::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("varDecl")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(VD->getLocation()))
        return;

      clang::TypeLoc TL = VD->getTypeSourceInfo()->getTypeLoc();
      if (auto TT = TL.getAs<clang::TemplateSpecializationTypeLoc>()) {
        const clang::TemplateSpecializationType *TST = TT.getTypePtr();
        if (TST->getTemplateName().getAsTemplateDecl()->getNameAsString().find("std::") == 0) {
          std::string originalName = TST->getTemplateName().getAsTemplateDecl()->getNameAsString();
          std::string mutatedName = originalName + "_invalid";
          std::string sourceText = stringutils::rangetoStr(*(Result.SourceManager), TT.getSourceRange());
          size_t pos = sourceText.find(originalName);
          if (pos != std::string::npos) {
            sourceText.replace(pos, originalName.length(), mutatedName);
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(TT.getSourceRange()), sourceText);
          }
        }
      }
    }
}
  
void MutatorFrontendAction_672::MutatorASTConsumer_672::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = varDecl(hasType(templateSpecializationType())).bind("varDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}