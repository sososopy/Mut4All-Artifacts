//source file
#include "../include/insert_recursive_template_infinite_448.h"

// ========================================================================================================
#define MUT448_OUTPUT 1

void MutatorFrontendAction_448::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FD->getLocation()))
            return;

        if (visitedFunctions.find(FD) != visitedFunctions.end())
            return;

        visitedFunctions.insert(FD);

        std::string templateCode = R"(
template<int N>
struct RecursiveTemplate {
    static const int value = RecursiveTemplate<N-1>::value;
};

template<>
struct RecursiveTemplate<0> {
    static const int value = 0;
};

void triggerRecursiveTemplate() {
    int x = RecursiveTemplate<1000000>::value; // Large value to trigger potential ICE
}
)";

        Rewrite.InsertTextAfterToken(FD->getEndLoc(), "\n/*mut448*/" + templateCode);
    }
}

void MutatorFrontendAction_448::MutatorASTConsumer_448::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl().bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}