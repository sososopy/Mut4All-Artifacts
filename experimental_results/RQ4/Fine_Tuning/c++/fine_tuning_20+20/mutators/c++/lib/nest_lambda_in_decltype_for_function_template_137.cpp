//source file
#include "../include/Nest_Lambda_In_Decltype_For_Function_Template_137.h"

// ========================================================================================================
#define MUT137_OUTPUT 1

void MutatorFrontendAction_137::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FuncTemp")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FT->getLocation()))
        return;
      auto params = FT->getTemplateParameters();
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             params->getSourceRange());
      auto first_param = params->getParam(0);
      auto first_type = stringutils::rangetoStr(*(Result.SourceManager),
                                                first_param->getSourceRange());
      string lambda = "[&]{return " + first_type + "{};}";
      content += ",/*mut137*/decltype(" + lambda + ')';
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(params->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_137::MutatorASTConsumer_137::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl().bind("FuncTemp");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}