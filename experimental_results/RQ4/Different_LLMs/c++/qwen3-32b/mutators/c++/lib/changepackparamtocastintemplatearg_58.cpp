//source file
#include "../include/ChangePackParamToCastInTemplateArg_58.h"

// ========================================================================================================
#define MUT58_OUTPUT 1

void MutatorFrontendAction_58::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TAL = Result.Nodes.getNodeAs<clang::TemplateArgumentLoc>("arg")) {
        if (!TAL || !Result.Context->getSourceManager().isWrittenInMainFile(TAL->getSourceRange().getBegin()))
            return;

        SourceRange range = TAL->getSourceRange();
        std::string originalArg = stringutils::rangetoStr(*Result.SourceManager, range);
        std::string mutatedArg = "static_cast<int>(" + originalArg + ")";
        Rewrite.ReplaceText(range, mutatedArg);
    }
}
  
void MutatorFrontendAction_58::MutatorASTConsumer_58::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = clang::ast_matchers::templateArgumentLoc().bind("arg");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}