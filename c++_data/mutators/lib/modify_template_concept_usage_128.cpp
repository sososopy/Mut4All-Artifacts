//source file
#include "../include/Modify_Template_Concept_Usage_128.h"

#define MUT128_OUTPUT 1

void MutatorFrontendAction_128::Callback::run(const clang::ast_matchers::MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionWithConcept")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (FD->hasBody()) {
            auto functionBody = clang::Lexer::getSourceText(clang::CharSourceRange::getTokenRange(FD->getBody()->getSourceRange()), *Result.SourceManager, clang::LangOptions(), 0);
            std::string ifConstruct = "\nif constexpr (C<T>) {\n  // Operation depending on T\n} else {\n  // Ambiguous path\n}\n";
            std::string functionBodyStr = std::string(functionBody);
            functionBodyStr.insert(functionBodyStr.find("{") + 1, ifConstruct);
            Rewrite.ReplaceText(clang::CharSourceRange::getTokenRange(FD->getBody()->getSourceRange()), functionBodyStr);
        }
    }
}

void MutatorFrontendAction_128::MutatorASTConsumer_128::HandleTranslationUnit(clang::ASTContext &Context) {
    clang::ast_matchers::MatchFinder matchFinder;
    using namespace clang::ast_matchers;
    DeclarationMatcher matcher = functionDecl(hasDescendant(declRefExpr(to(functionDecl().bind("FunctionWithConcept")))));
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}