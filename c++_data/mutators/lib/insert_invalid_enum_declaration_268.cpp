//source file
#include "../include/Mutator_268.h"

// ========================================================================================================
#define MUT268_OUTPUT 1

void MutatorFrontendAction_268::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ED = Result.Nodes.getNodeAs<clang::EnumDecl>("EnumDecl")) {
        if (!ED || !Result.Context->getSourceManager().isWrittenInMainFile(
                       ED->getLocation()))
            return;

        auto enumSource = stringutils::rangetoStr(*(Result.SourceManager), ED->getSourceRange());
        std::string invalidEnum = "int"; // Example of invalid enumerator
        if (enumSource.find(invalidEnum) == std::string::npos) {
            size_t pos = enumSource.rfind('}');
            if (pos != std::string::npos) {
                enumSource.insert(pos, ", " + invalidEnum);
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(ED->getSourceRange()), enumSource);
            }
        }
    } else if (Result.Nodes.getNodeAs<clang::TranslationUnitDecl>("TranslationUnit")) {
        SourceLocation insertLoc = Result.Context->getTranslationUnitDecl()->getBeginLoc();
        std::string newEnum = "\nenum Status { Valid, Invalid, 123Invalid }; // Invalid enumerator\n";
        Rewrite.InsertText(insertLoc, newEnum, true, true);
    }
}

void MutatorFrontendAction_268::MutatorASTConsumer_268::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher enumMatcher = enumDecl().bind("EnumDecl");
    DeclarationMatcher translationUnitMatcher = translationUnitDecl().bind("TranslationUnit");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(enumMatcher, &callback);
    matchFinder.addMatcher(translationUnitMatcher, &callback);
    matchFinder.matchAST(Context);
}