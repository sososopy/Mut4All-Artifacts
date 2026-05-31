//source file
#include "../include/Modify_Template_Parameter_Pack_Instantiation_204.h"

// ========================================================================================================
#define MUT204_OUTPUT 1

void MutatorFrontendAction_204::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (!FD->isTemplateInstantiation())
        return;
      if (FD->getTemplateSpecializationArgs() == nullptr)
        return;
      auto args = FD->getTemplateSpecializationArgs()->asArray();
      for (auto arg : args) {
        if (arg.getKind() == TemplateArgument::Pack) {
          auto name = FD->getNameAsString();
          name += "<>";
          Rewrite.ReplaceText(FD->getSourceRange(), name);
        }
      }
    }
}
  
void MutatorFrontendAction_204::MutatorASTConsumer_204::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionDecl().bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}