//source file
#include "../include/Function_Parameter_Shadowing_with_External_Declaration_239.h"

// ========================================================================================================
#define MUT239_OUTPUT 1

void MutatorFrontendAction_239::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->isThisDeclarationADefinition() == false)
        return;
      if (FD->hasBody() == false)
        return;
      auto params = FD->parameters();
      string ins = "";
      for (auto param : params) {
        auto param_name = param->getNameAsString();
        auto param_type = param->getType().getAsString();
        ins += "extern " + param_type + " " + param_name + ";\n";
      }
      llvm::outs() << ins;
      if (FD->getBody()->getBeginLoc().isInvalid())
        return;
      Rewrite.ReplaceText(FD->getBody()->getBeginLoc(), 0, "/*mut239*/" + ins);
    }
}
  
void MutatorFrontendAction_239::MutatorASTConsumer_239::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionDecl().bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}