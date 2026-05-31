//source file
#include "../include/insert_recursive_template_instantiation_452.h"

// ========================================================================================================
#define MUT452_OUTPUT 1

void MutatorFrontendAction_452::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;
        if (processedFunctions.find(FD) != processedFunctions.end())
            return;
        processedFunctions.insert(FD);

        auto funcName = FD->getNameAsString();
        if (funcName.empty())
            return;

        std::string recursiveTemplate = R"(
template<int N>
struct RecursiveTemplate {
    static void call() {
        if constexpr (N > 0) {
            RecursiveTemplate<N - 1>::call();
        }
    }
};

)";

        std::string insertion = recursiveTemplate + "/*mut452*/RecursiveTemplate<10>::call();\n";
        if (FD->hasBody()) {
            auto bodyRange = FD->getBody()->getSourceRange();
            Rewrite.InsertTextAfterToken(bodyRange.getBegin(), insertion);
        }
    }
}

void MutatorFrontendAction_452::MutatorASTConsumer_452::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isDefinition()).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}