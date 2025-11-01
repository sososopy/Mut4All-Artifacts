//source file
#include "../include/Modify_Template_Class_Specialization_162.h"

// ========================================================================================================
#define MUT162_OUTPUT 1

void MutatorFrontendAction_162::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TS = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("TemplateSpecialization")) {
      if (!TS || !Result.Context->getSourceManager().isWrittenInMainFile(TS->getLocation()))
        return;

      auto specialization = stringutils::rangetoStr(*(Result.SourceManager), TS->getSourceRange());
      std::string modifiedSpecialization = specialization;

      // Perform mutation by removing the use of template parameters in the specialization
      // Here we assume the specialization uses a single template parameter and we remove its usage
      size_t parameterPos = modifiedSpecialization.find("<");
      if (parameterPos != std::string::npos) {
        size_t endPos = modifiedSpecialization.find(">", parameterPos);
        if (endPos != std::string::npos) {
          modifiedSpecialization.erase(parameterPos + 1, endPos - parameterPos - 1);
        }
      }

      Rewrite.ReplaceText(CharSourceRange::getTokenRange(TS->getSourceRange()), modifiedSpecialization);
    }
}
  
void MutatorFrontendAction_162::MutatorASTConsumer_162::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateSpecializationDecl().bind("TemplateSpecialization");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}