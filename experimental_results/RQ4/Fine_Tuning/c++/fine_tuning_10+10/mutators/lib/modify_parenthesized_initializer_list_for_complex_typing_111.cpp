//source file
#include "../include/modify_parenthesized_initializer_list_for_complex_typing_111.h"

// ========================================================================================================
#define MUT111_OUTPUT 1

void MutatorFrontendAction_111::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ILE = Result.Nodes.getNodeAs<clang::InitListExpr>("InitList")) {
        if (!ILE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ILE->getBeginLoc()))
            return;

        auto originalText = stringutils::rangetoStr(*(Result.SourceManager), ILE->getSourceRange());

        std::string modifiedText = "({";  // Start with an extra parenthesis

        for (unsigned i = 0; i < ILE->getNumInits(); ++i) {
            if (i > 0) modifiedText += ", ";
            if (i == 1) {
                modifiedText += "\"text\"";  // Insert a string literal as an incompatible type
            } else if (i == 2) {
                modifiedText += "3.0";  // Insert a double as another incompatible type
            } else {
                modifiedText += stringutils::rangetoStr(*(Result.SourceManager), ILE->getInit(i)->getSourceRange());
            }
        }

        modifiedText += "})";  // Close the extra parenthesis

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(ILE->getSourceRange()), modifiedText);
    }
}
  
void MutatorFrontendAction_111::MutatorASTConsumer_111::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = initListExpr(hasType(isInteger())).bind("InitList");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}