//source file
#include "../include/Insert_Export_Template_66.h"

// ========================================================================================================
#define MUT66_OUTPUT 1

void MutatorFrontendAction_66::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::TemplateDecl>("template")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;
        SourceLocation loc = TD->getLocation();
        Rewrite.InsertText(loc, "export ", true, true);
    }
}

void MutatorFrontendAction_66::MutatorASTConsumer_66::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = clang::ast_matchers::anyOf(
        clang::ast_matchers::functionTemplateDecl().bind("template"),
        clang::ast_matchers::classTemplateDecl().bind("template")
    );
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}