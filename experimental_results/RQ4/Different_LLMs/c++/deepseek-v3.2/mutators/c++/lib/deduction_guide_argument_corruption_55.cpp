//source file
#include "../include/Mutator_Deduction_Guide_Argument_Corruption_55.h"

// ========================================================================================================
#define MUT55_OUTPUT 1

void MutatorFrontendAction_55::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *DG = Result.Nodes.getNodeAs<clang::CXXDeductionGuideDecl>("DeductionGuide")) {
      //Filter nodes in header files
      if (!DG || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DG->getLocation()))
        return;
      
      // Get the associated template declaration
      auto *Template = DG->getDescribedFunctionTemplate();
      if (!Template) return;
      
      // Get template parameters
      auto *TemplateParams = Template->getTemplateParameters();
      if (!TemplateParams || TemplateParams->size() == 0) return;
      
      // Choose a random template parameter
      unsigned ParamIndex = getrandom::getRandomIndex(TemplateParams->size() - 1);
      auto *Param = TemplateParams->getParam(ParamIndex);
      if (!Param) return;
      
      // Get parameter name
      std::string ParamName = Param->getNameAsString();
      if (ParamName.empty()) {
        // If parameter has no name, use default names
        const char* DefaultNames[] = {"T", "U", "V", "W", "X", "Y", "Z"};
        unsigned DefaultIndex = getrandom::getRandomIndex(6);
        ParamName = DefaultNames[DefaultIndex];
      }
      
      // Get the original parameter list source range
      auto *FuncDecl = DG->getAsFunction();
      if (!FuncDecl) return;
      
      auto ParamRange = FuncDecl->getParametersSourceRange();
      if (ParamRange.isInvalid()) return;
      
      // Get source manager
      SourceManager &SM = *Result.SourceManager;
      
      // Get the original parameter list text
      std::string OriginalParams = stringutils::rangetoStr(SM, ParamRange);
      
      // Choose mutation type: either "ParamName()" or just "ParamName"
      unsigned MutationType = getrandom::getRandomIndex(1);
      std::string NewParams;
      
      if (MutationType == 0) {
        NewParams = ParamName + "()";
      } else {
        NewParams = ParamName;
      }
      
      // Perform mutation on the source code text by applying string replacement
      // Replace the original AST node with the mutated one
      Rewrite.ReplaceText(ParamRange, NewParams);
    }
}
  
void MutatorFrontendAction_55::MutatorASTConsumer_55::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxDeductionGuideDecl().bind("DeductionGuide");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}