//source file
#include "../include/Incomplete_Template_Parameter_In_Consteval_Function_306.h"

// ========================================================================================================
#define MUT306_OUTPUT 1

void MutatorFrontendAction_306::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::TemplateDecl>("ConstevalTemplate")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;

        if (!clang::isa<clang::FunctionTemplateDecl>(TD))
            return;

        auto *FD = TD->getAsFunction();
        if (!FD || !FD->isConsteval())
            return;

        SourceRange templateRange = TD->getSourceRange();
        if (templateRange.isInvalid())
            return;

        int choice = getrandom::getRandomIndex(2);
        if (choice == 0) {
            Rewrite.ReplaceText(templateRange.getEnd(), 1, "");
        } else {
            Rewrite.ReplaceText(templateRange, "template <>");
        }
    }
}

void MutatorFrontendAction_306::MutatorASTConsumer_306::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = 
        functionTemplateDecl(
            hasDescendant(
                functionDecl(
                    isConsteval()
                ).bind("ConstevalFunction")
            )
        ).bind("ConstevalTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}