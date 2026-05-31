//source file
#include "../include/insert_unused_template_instantiation_2.h"

// ========================================================================================================
#define MUT2_OUTPUT 1

void MutatorFrontendAction_2::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FD->getLocation()))
            return;
        if (!FD->hasBody())
            return;

        functionDecls.push_back(FD);
    }

    if (auto *TL = Result.Nodes.getNodeAs<clang::TranslationUnitDecl>("TranslationUnit")) {
        if (!TL || !Result.Context->getSourceManager().isWrittenInMainFile(
                       TL->getLocation()))
            return;

        if (functionDecls.empty())
            return;

        std::string templateCode = "template<typename T> void unusedTemplate() {}\n";
        std::string instantiationCode = "unusedTemplate<int>();\n";

        for (auto *FD : functionDecls) {
            SourceLocation insertLoc = FD->getBodyRBrace();
            Rewrite.InsertTextAfterToken(insertLoc, instantiationCode);
        }

        SourceLocation insertLoc = TL->getBeginLoc();
        Rewrite.InsertTextBefore(insertLoc, templateCode);
    }
}

void MutatorFrontendAction_2::MutatorASTConsumer_2::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher functionMatcher = functionDecl().bind("Functions");
    DeclarationMatcher translationUnitMatcher = translationUnitDecl().bind("TranslationUnit");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(functionMatcher, &callback);
    matchFinder.addMatcher(translationUnitMatcher, &callback);
    matchFinder.matchAST(Context);
}