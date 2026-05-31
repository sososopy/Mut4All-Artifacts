//source file
#include "../include/replace_nested_template_type_46.h"

// ========================================================================================================
#define MUT46_OUTPUT 1

void MutatorFrontendAction_46::Callback::run(const MatchFinder::MatchResult &Result) {
    if (const auto *NT = Result.Nodes.getNodeAs<clang::NestedNameSpecifierLoc>("nestedType")) {
        if (!NT || !Result.Context->getSourceManager().isWrittenInMainFile(NT->getBeginLoc()))
            return;

        auto sourceRange = NT->getSourceRange();
        auto sourceText = Lexer::getSourceText(CharSourceRange::getTokenRange(sourceRange), 
                                               Result.Context->getSourceManager(), 
                                               Result.Context->getLangOpts());

        std::string mutatedText = sourceText.str() + "::nonexistent_type";
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), mutatedText);
    }
}
  
void MutatorFrontendAction_46::MutatorASTConsumer_46::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = nestedNameSpecifierLoc().bind("nestedType");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}