//source file
#include "../include/insert_semicolon_in_namespace_decl_148.h"

// ========================================================================================================
#define MUT148_OUTPUT 1

void MutatorFrontendAction_148::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ND = Result.Nodes.getNodeAs<clang::NamespaceDecl>("NamespaceDecl")) {
        if (!ND || !Result.Context->getSourceManager().isWrittenInMainFile(ND->getLocation()))
            return;
        
        auto &SM = Result.Context->getSourceManager();
        auto Loc = ND->getLocation();
        auto Range = ND->getSourceRange();
        
        if (SM.isInMainFile(Loc)) {
            auto Text = stringutils::rangetoStr(SM, Range);
            size_t bracePos = Text.find('{');
            if (bracePos != std::string::npos && Text.find('\n', 0) < bracePos) {
                std::string MutatedText = Text.substr(0, bracePos) + ";" + Text.substr(bracePos);
                Rewrite.ReplaceText(Range, MutatedText);
            }
        }
    }
}

void MutatorFrontendAction_148::MutatorASTConsumer_148::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = namespaceDecl().bind("NamespaceDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}