//source file
#include "../include/introduce_conflicting_linkage_44.h"

// ========================================================================================================
#define MUT44_OUTPUT 1

void MutatorFrontendAction_44::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionWithParam")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
        return;

      if (FD->getNumParams() > 0 && FD->hasBody()) {
        const ParmVarDecl *param = FD->getParamDecl(0);
        std::string paramName = param->getNameAsString();
        std::string paramType = param->getType().getAsString();

        std::string externDecl = "extern " + paramType + " " + paramName + "; /*mut44*/\n";
        SourceLocation insertLoc = FD->getBody()->getBeginLoc().getLocWithOffset(1);

        Rewrite.InsertText(insertLoc, externDecl, true, true);
      }
    }
}
  
void MutatorFrontendAction_44::MutatorASTConsumer_44::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(hasParameter(0, parmVarDecl())).bind("FunctionWithParam");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}