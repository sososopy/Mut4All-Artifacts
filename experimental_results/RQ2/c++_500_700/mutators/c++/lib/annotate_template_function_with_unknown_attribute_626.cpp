//source file
#include "../include/annotate_template_function_with_unknown_attribute_626.h"

// ========================================================================================================
#define MUT626_OUTPUT 1

void MutatorFrontendAction_626::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      auto functionDecl = FD->getTemplatedDecl();
      if (!functionDecl)
        return;

      auto functionSourceRange = functionDecl->getSourceRange();
      std::string modifiedFunction = stringutils::rangetoStr(*(Result.SourceManager), functionSourceRange);
      
      std::string annotation = "[[unknown_attribute]]\n";
      modifiedFunction = annotation + modifiedFunction;

      Rewrite.ReplaceText(CharSourceRange::getTokenRange(functionSourceRange), modifiedFunction);
    }
}
  
void MutatorFrontendAction_626::MutatorASTConsumer_626::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl().bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}