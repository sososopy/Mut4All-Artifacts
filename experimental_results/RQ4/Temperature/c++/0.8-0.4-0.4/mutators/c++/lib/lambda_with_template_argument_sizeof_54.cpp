//source file
#include "../include/lambda_with_template_argument_sizeof_54.h"

// ========================================================================================================
#define MUT54_OUTPUT 1

void MutatorFrontendAction_54::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FT->getLocation()))
        return;

      if (FT->getTemplatedDecl()->hasBody()) {
        auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   FT->getTemplatedDecl()->getSourceRange());
        auto pos = declaration.find("{");
        if (pos != std::string::npos) {
          std::string lambdaExpr = "\n/*mut54*/auto lambda = [=]() { return t; }; int size = sizeof(lambda);";
          declaration.insert(pos + 1, lambdaExpr);
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(FT->getTemplatedDecl()->getSourceRange()), declaration);
        }
      }
    }
}
  
void MutatorFrontendAction_54::MutatorASTConsumer_54::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl(has(cxxMethodDecl())).bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}