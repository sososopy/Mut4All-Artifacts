//source file
#include "../include/Insert_Cpp_Cast_In_C_Function_356.h"

// ========================================================================================================
#define MUT356_OUTPUT 1

void MutatorFrontendAction_356::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->getNumParams() > 0) {
        auto *param = FD->getParamDecl(0);
        if (param) {
          auto paramType = param->getType().getAsString();
          std::string mutatedParamType = "const_cast<" + paramType + ">(" + param->getNameAsString() + ")";
          Rewrite.ReplaceText(param->getSourceRange(), mutatedParamType);
        }
      }
    }
}
  
void MutatorFrontendAction_356::MutatorASTConsumer_356::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl().bind("FunctionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}