//source file
#include "../include/Replace_NonVoid_Function_Return_Type_With_Auto_269.h"

// ========================================================================================================
#define MUT269_OUTPUT 1

void MutatorFrontendAction_269::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->getReturnType()->isVoidType())
        return;
      if (FD->isTemplateInstantiation())
        return;
      if (FD->isDependentContext())
        return;
      if (FD->getReturnType()->isDependentType())
        return;

      auto return_type = stringutils::rangetoStr(
          *(Result.SourceManager), FD->getReturnTypeSourceRange());
      if (return_type == "auto")
        return;
      if (FD->getType().getAsString().find("auto") != std::string::npos) {
        auto trailing_return_type = stringutils::rangetoStr(
            *(Result.SourceManager), FD->getReturnTypeSourceRange());
        if (trailing_return_type == return_type) {
          Rewrite.ReplaceText(
              CharSourceRange::getTokenRange(
                  FD->getReturnTypeSourceRange()),
              "/*mut269*/auto");
        }
      } else {
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FD->getReturnTypeSourceRange()),
            "/*mut269*/auto");
      }
    }
}
  
void MutatorFrontendAction_269::MutatorASTConsumer_269::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl().bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}