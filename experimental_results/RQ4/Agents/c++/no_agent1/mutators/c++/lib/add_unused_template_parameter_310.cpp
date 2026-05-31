//source file
#include "../include/add_unused_template_parameter_310.h"

// ========================================================================================================
#define MUT310_OUTPUT 1

void MutatorFrontendAction_310::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
            return;

        if (FD->isTemplateInstantiation() || FD->getTemplatedKind() != clang::FunctionDecl::TK_NonTemplate)
            return;

        auto functionSource = stringutils::rangetoStr(*(Result.SourceManager), FD->getSourceRange());

        std::string templateParam = "template<typename UnusedParam>\n";
        functionSource.insert(0, templateParam);

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), functionSource);
    }
}

void MutatorFrontendAction_310::MutatorASTConsumer_310::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isDefinition()).bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}