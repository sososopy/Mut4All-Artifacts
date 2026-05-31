//source file
#include "../include/Modify_Class_Template_Argument_Deduction_33.h"

// ========================================================================================================
#define MUT33_OUTPUT 1

void MutatorFrontendAction_33::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("ClassTemplate")) {
      //Filter nodes in header files
      if (!CT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CT->getLocation()))
        return;

      //Get the source code text of target node
      auto className = CT->getNameAsString();
      auto templateParams = CT->getTemplateParameters();
      if (templateParams->size() != 1)
        return;

      auto paramName = templateParams->getParam(0)->getNameAsString();
      std::string deductionGuide = className + "(const " + paramName + " &object) : object(object) {}";

      //Perform mutation on the source code text by applying string replacement
      std::string mutation = "\n/*mut33*/" + deductionGuide + "\n";
      Rewrite.InsertTextAfterToken(CT->getEndLoc(), mutation);
    }
}
  
void MutatorFrontendAction_33::MutatorASTConsumer_33::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateDecl(has(cxxRecordDecl(has(cxxConstructorDecl())))).bind("ClassTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}