//source file
#include "../include/default_argument_in_template_instantiation_65.h"

// ========================================================================================================
#define MUT65_OUTPUT 1

void MutatorFrontendAction_65::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplates")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FT->getLocation()))
        return;
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), FT->getSourceRange());
      if (content.find('(') != string::npos)
        content.insert(content.find('('), "int mut65=10,");
      else
        return;
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FT->getSourceRange()),
                          content);
      cur_templates.push_back(FT);
    } else if (auto *CL = Result.Nodes.getNodeAs<clang::CallExpr>("CallExpr")) {
      if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CL->getBeginLoc()))
        return;
      auto callee = CL->getCalleeDecl();
      if (callee == nullptr)
        return;
      if (callee->isTemplateDecl() == false)
        return;
      auto target = callee->getAsFunction();
      if (target == nullptr)
        return;
      for (std::size_t i = 0; i < cur_templates.size(); ++i) {
        if (cur_templates[i]->getTemplatedDecl() == target) {
          auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                                 CL->getSourceRange());
          if (content.rfind(')') != string::npos)
            content.insert(content.rfind(')'), ",mut65");
          Rewrite.ReplaceText(
              CharSourceRange::getTokenRange(CL->getSourceRange()), content);
        }
      }
    }
}
  
void MutatorFrontendAction_65::MutatorASTConsumer_65::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionTemplateDecl().bind("FunctionTemplates");
    auto call_matcher = callExpr().bind("CallExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(call_matcher, &callback);
    matchFinder.matchAST(Context);
}