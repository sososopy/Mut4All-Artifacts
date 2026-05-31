//source file
#include "../include/convert_function_to_template_22.h"

// ========================================================================================================
#define MUT22_OUTPUT 1

void MutatorFrontendAction_22::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FD->getLocation()))
            return;

        if (FD->isTemplateInstantiation() || FD->isOverloadedOperator())
            return;

        auto functionSource = stringutils::rangetoStr(*(Result.SourceManager), FD->getSourceRange());

        std::string templatePrefix = "template<typename T> ";
        std::string newFunctionSource = templatePrefix + functionSource;

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), newFunctionSource);
    }
}

void MutatorFrontendAction_22::MutatorASTConsumer_22::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isDefinition(), unless(isTemplateInstantiation())).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}