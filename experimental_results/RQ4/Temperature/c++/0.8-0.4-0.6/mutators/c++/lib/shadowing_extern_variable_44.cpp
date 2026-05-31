//source file
#include "../include/shadowing_extern_variable_44.h"

// ========================================================================================================
#define MUT44_OUTPUT 1

void MutatorFrontendAction_44::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionWithParams")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
        return;

      if (FD->getNumParams() > 0) {
        auto param = FD->getParamDecl(0);
        if (param) {
          std::string paramName = param->getNameAsString();
          SourceLocation insertLoc = FD->getBody()->getBeginLoc().getLocWithOffset(1);
          std::string externDecl = "extern int " + paramName + "; /*mut44*/\n";
          Rewrite.InsertText(insertLoc, externDecl, true, true);
        }
      }
    }
}
  
void MutatorFrontendAction_44::MutatorASTConsumer_44::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(hasBody(stmt()), hasAnyParameter(parmVarDecl())).bind("FunctionWithParams");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}