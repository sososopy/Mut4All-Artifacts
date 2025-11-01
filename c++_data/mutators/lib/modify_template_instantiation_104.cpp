//source file
#include "../include/Modify_Template_Instantiation_104.h"

// ========================================================================================================
#define MUT104_OUTPUT 1

void MutatorFrontendAction_104::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("TemplateFunction")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;

      for (auto *Spec : MT->specializations()) {
        if (Spec->getTemplateSpecializationKind() == TSK_ExplicitSpecialization) {
          auto specRange = Spec->getSourceRange();
          auto specText = stringutils::rangetoStr(*(Result.SourceManager), specRange);

          if (specText.find('<') != std::string::npos) {
            auto insertPos = specText.find('>');
            specText.insert(insertPos, ", char");
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(specRange), specText);
          }
        }
      }
    }
}
  
void MutatorFrontendAction_104::MutatorASTConsumer_104::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl().bind("TemplateFunction");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}