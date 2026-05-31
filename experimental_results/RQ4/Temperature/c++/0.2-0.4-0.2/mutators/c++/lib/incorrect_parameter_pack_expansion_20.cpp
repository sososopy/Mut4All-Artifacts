//source file
#include "../include/incorrect_parameter_pack_expansion_20.h"

// ========================================================================================================
#define MUT20_OUTPUT 1

void MutatorFrontendAction_20::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FuncTemplate")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->getTemplatedDecl()->hasBody()) {
        auto body = FD->getTemplatedDecl()->getBody();
        auto bodyText = stringutils::rangetoStr(*(Result.SourceManager), body->getSourceRange());

        std::string mutation = "auto invalidExpansion = sizeof...(args) + args...; // Incorrect expansion\n";
        bodyText.insert(bodyText.find("{") + 1, "\n/*mut20*/" + mutation);

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(body->getSourceRange()), bodyText);
      }
    }
}
  
void MutatorFrontendAction_20::MutatorASTConsumer_20::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl(has(templateTypeParmDecl().bind("ParamPack"))).bind("FuncTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}