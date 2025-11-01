//source file
#include "../include/misplaced_malformed_requires_clause_355.h"

// ========================================================================================================
#define MUT355_OUTPUT 1

void MutatorFrontendAction_355::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (FD->hasBody()) {
            auto declaration = stringutils::rangetoStr(*(Result.SourceManager), FD->getSourceRange());
            auto pos = declaration.find("{");
            if (pos != std::string::npos) {
                std::string malformedRequires = "\nrequires { requires int; } // Misplaced and malformed requires clause\n";
                declaration.insert(pos + 1, malformedRequires);
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), declaration);
            }
        }
    }
}

void MutatorFrontendAction_355::MutatorASTConsumer_355::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(unless(isExpansionInSystemHeader())).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}