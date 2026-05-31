//source file
#include "../include/insert_recursive_template_instantiation_480.h"

// ========================================================================================================
#define MUT480_OUTPUT 1

void MutatorFrontendAction_480::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FD->getLocation()))
            return;

        if (FD->hasBody()) {
            auto functionBody = stringutils::rangetoStr(*(Result.SourceManager),
                                                        FD->getBody()->getSourceRange());

            std::string templateCode = R"(
template<int N>
struct RecursiveTemplate {
    static constexpr int value = RecursiveTemplate<N - 1>::value;
};

template<>
struct RecursiveTemplate<0> {
    static constexpr int value = 0;
};

int triggerRecursion() {
    return RecursiveTemplate<1000>::value;
}
)";

            functionBody.insert(functionBody.find("{") + 1, "\n/*mut480*/" + templateCode + "\n");
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getBody()->getSourceRange()), functionBody);
        }
    }
}

void MutatorFrontendAction_480::MutatorASTConsumer_480::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isDefinition()).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}