//source file
#include "../include/Modify_Template_Syntax_293.h"

// ========================================================================================================
#define MUT293_OUTPUT 1

void MutatorFrontendAction_293::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TI = Result.Nodes.getNodeAs<clang::TemplateSpecializationType>("TemplateInst")) {
      //Filter nodes in header files
      if (!TI || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TI->getTemplateName().getAsTemplateDecl()->getLocation()))
        return;

      //Get the source code text of target node
      auto args = TI->template_arguments();
      if (args.size() > 0 && args[0].getKind() == TemplateArgument::Integral) {
        //Perform mutation on the source code text by applying string replacement
        auto argRange = TI->getTemplateName().getAsTemplateDecl()->getLocation();
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(argRange), "'a'");
      }
    }
}
  
void MutatorFrontendAction_293::MutatorASTConsumer_293::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    TypeMatcher matcher = templateSpecializationType().bind("TemplateInst");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}