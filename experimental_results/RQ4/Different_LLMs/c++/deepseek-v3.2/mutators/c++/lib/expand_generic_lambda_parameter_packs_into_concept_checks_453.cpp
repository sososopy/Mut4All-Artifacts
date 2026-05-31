//source file
#include "../include/Expand_Generic_Lambda_Parameter_Packs_Into_Concept_Checks_453.h"

// ========================================================================================================
#define MUT453_OUTPUT 1

void MutatorFrontendAction_453::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *L = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambdaExpr")) {
      //Filter nodes in header files
      if (!L || !Result.Context->getSourceManager().isWrittenInMainFile(
                     L->getBeginLoc()))
        return;
      
      // Check if lambda is generic (has a parameter pack)
      const clang::CXXRecordDecl *LambdaClass = L->getLambdaClass();
      if (!LambdaClass) return;
      
      const clang::FunctionDecl *CallOperator = LambdaClass->getLambdaCallOperator();
      if (!CallOperator) return;
      
      // Check if call operator has a parameter pack
      bool hasParamPack = false;
      for (unsigned i = 0; i < CallOperator->getNumParams(); ++i) {
        const clang::ParmVarDecl *Param = CallOperator->getParamDecl(i);
        if (Param->getType()->isTemplateTypeParmType()) {
          const clang::TemplateTypeParmType *TTP = Param->getType()->getAs<clang::TemplateTypeParmType>();
          if (TTP && TTP->isParameterPack()) {
            hasParamPack = true;
            break;
          }
        }
      }
      
      if (!hasParamPack) return;
      
      // Get the source code text of target node
      auto lambdaRange = L->getSourceRange();
      std::string lambdaText = stringutils::rangetoStr(*(Result.SourceManager), lambdaRange);
      
      // Find available concept names in the current context
      // We need to collect concept declarations from the AST
      if (availableConceptNames.empty()) {
        // Collect concept names from the translation unit
        clang::ASTContext &Context = *Result.Context;
        clang::TranslationUnitDecl *TU = Context.getTranslationUnitDecl();
        for (clang::Decl *D : TU->decls()) {
          if (auto *ConceptDecl = dyn_cast<clang::ConceptDecl>(D)) {
            availableConceptNames.push_back(ConceptDecl->getNameAsString());
          }
        }
      }
      
      if (availableConceptNames.empty()) return;
      
      // Choose a random concept to use
      size_t conceptIndex = getrandom::getRandomIndex(availableConceptNames.size() - 1);
      std::string conceptName = availableConceptNames[conceptIndex];
      
      // Perform mutation on the source code text by applying string replacement
      // Find the lambda body start position
      size_t bodyStart = lambdaText.find('{');
      if (bodyStart == std::string::npos) return;
      
      // Insert concept expansion code at the beginning of the lambda body
      std::string expansionCode = "\n    using dummy = int[];\n    (void)dummy{ 0, (requires { requires " + conceptName + "<decltype(xs)>; } ? 0 : 0)... };";
      
      // Replace xs with the actual parameter pack variable name
      // We need to get the parameter pack variable name from the lambda
      std::string paramPackName = "xs"; // Default name
      for (unsigned i = 0; i < CallOperator->getNumParams(); ++i) {
        const clang::ParmVarDecl *Param = CallOperator->getParamDecl(i);
        if (Param->getType()->isTemplateTypeParmType()) {
          const clang::TemplateTypeParmType *TTP = Param->getType()->getAs<clang::TemplateTypeParmType>();
          if (TTP && TTP->isParameterPack()) {
            paramPackName = Param->getNameAsString();
            break;
          }
        }
      }
      
      // Replace xs in expansionCode with actual parameter pack name
      size_t xsPos = expansionCode.find("xs");
      if (xsPos != std::string::npos) {
        expansionCode.replace(xsPos, 2, paramPackName);
      }
      
      // Insert the expansion code
      lambdaText.insert(bodyStart + 1, expansionCode);
      
      // Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(lambdaRange), lambdaText);
    }
}
  
void MutatorFrontendAction_453::MutatorASTConsumer_453::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = lambdaExpr().bind("lambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}