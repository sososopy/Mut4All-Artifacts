//source file
#include "../include/insert_unused_template_parameter_456.h"

// ========================================================================================================
#define MUT456_OUTPUT 1

void MutatorFrontendAction_456::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FD->getLocation()))
            return;

        if (FD->isTemplateInstantiation() || FD->isDependentContext())
            return;

        std::string functionText = stringutils::rangetoStr(*(Result.SourceManager), FD->getSourceRange());

        if (FD->getTemplatedKind() == FunctionDecl::TK_NonTemplate) {
            std::string templateParam = "template<typename UnusedType>\n";
            functionText.insert(0, templateParam);
            functionText.insert(functionText.find("{") + 1, "\nUnusedType unusedVar;\n");
        }

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), functionText);
    }
}

void MutatorFrontendAction_456::MutatorASTConsumer_456::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isDefinition()).bind("FunctionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}