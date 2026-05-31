//source file
#include "../include/Modify_Template_Deduction_Guide_445.h"

// ========================================================================================================
#define MUT445_OUTPUT 1

void MutatorFrontendAction_445::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TDG = Result.Nodes.getNodeAs<clang::ClassTemplatePartialSpecializationDecl>("TemplateGuide")) {
      if (!TDG || !Result.Context->getSourceManager().isWrittenInMainFile(TDG->getLocation()))
        return;

      auto deductionGuide = stringutils::rangetoStr(*(Result.SourceManager), TDG->getSourceRange());
      
      // Perform mutation by altering template parameter order
      size_t arrowPos = deductionGuide.find("->");
      if (arrowPos != std::string::npos) {
          std::string beforeArrow = deductionGuide.substr(0, arrowPos);
          std::string afterArrow = deductionGuide.substr(arrowPos);
          
          // Swap template parameters in the deduction guide
          std::string mutatedGuide = beforeArrow + "->" + "bar<U, T>;";
          
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(TDG->getSourceRange()), mutatedGuide);
      }
    }
}
  
void MutatorFrontendAction_445::MutatorASTConsumer_445::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplatePartialSpecializationDecl().bind("TemplateGuide");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}