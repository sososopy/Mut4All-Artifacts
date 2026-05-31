//source file
#include "../include/Replace_Template_Parameter_With_Typename_86.h"

// ========================================================================================================
#define MUT86_OUTPUT 1

void MutatorFrontendAction_86::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("ClassTemplate")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto templateParams = MT->getTemplateParameters();
      for (auto it = templateParams->begin(); it != templateParams->end(); ++it) {
        if (auto *tparam = dyn_cast<clang::TemplateTypeParmDecl>(*it)) {
          // Perform mutation on the source code text by applying string replacement
          std::string typenameName = "typename";
          std::string replacement = typenameName + " " + tparam->getNameAsString();
          Rewrite.ReplaceText(tparam->getSourceRange(), replacement);
        }
      }
    }
}
  
void MutatorFrontendAction_86::MutatorASTConsumer_86::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateDecl().bind("ClassTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}