//source file
#include "../include/insert_unused_template_parameter_488.h"

// ========================================================================================================
#define MUT488_OUTPUT 1

void MutatorFrontendAction_488::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FD->getLocation()))
            return;

        if (FD->isTemplateInstantiation() || FD->isFunctionTemplateSpecialization())
            return;

        if (!FD->hasBody())
            return;

        auto funcText = stringutils::rangetoStr(*(Result.SourceManager), FD->getSourceRange());
        std::string templateParam = "template<typename UnusedParam>\n";
        funcText.insert(0, templateParam);

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), funcText);
    }
}

void MutatorFrontendAction_488::MutatorASTConsumer_488::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl().bind("FunctionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}