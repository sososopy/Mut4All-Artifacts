//source file
#include "../include/insert_unnamed_namespace_with_function_114.h"

// ========================================================================================================
#define MUT114_OUTPUT 1

void MutatorFrontendAction_114::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->hasBody()) {
        auto functionBody = stringutils::rangetoStr(*(Result.SourceManager),
                                                    FD->getSourceRange());
        std::string unnamedNamespace = "\nnamespace {\nvoid hiddenFunction() {\n";
        unnamedNamespace += functionBody;
        unnamedNamespace += "\n}\n}\n";

        Rewrite.InsertTextBefore(FD->getBeginLoc(), "/*mut114*/" + unnamedNamespace);
      }
    }
}
  
void MutatorFrontendAction_114::MutatorASTConsumer_114::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isDefinition()).bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}