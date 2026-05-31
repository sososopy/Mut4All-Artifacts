//source file
#include "../include/template_specialization_confusion_513.h"

// ========================================================================================================
#define MUT513_OUTPUT 1

void MutatorFrontendAction_513::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("TemplateFunc")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FD->getLocation()))
            return;

        auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   FD->getSourceRange());

        size_t pos = declaration.find("std::enable_if_t<");
        if (pos != std::string::npos) {
            pos = declaration.find(",", pos);
            if (pos != std::string::npos) {
                std::string nestedCalc = "template <typename X> struct NestedCalc { static const bool value = sizeof(X) > 0; };";
                std::string newCondition = "std::enable_if_t<(Condition && NestedCalc<U>::value), int>";

                declaration.insert(0, nestedCalc + "\n");
                declaration.replace(pos + 1, std::string::npos, newCondition);
            }
        }

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), declaration);
    }
}

void MutatorFrontendAction_513::MutatorASTConsumer_513::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(
        has(
            templateTypeParmDecl().bind("TemplateFunc")
        )
    ).bind("TemplateFunc");

    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}