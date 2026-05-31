//source file
#include "../include/insert_unused_template_parameter_143.h"

// ========================================================================================================
#define MUT143_OUTPUT 1

void MutatorFrontendAction_143::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FD->getLocation()))
            return;

        if (processedFunctions.find(FD) != processedFunctions.end())
            return;

        processedFunctions.insert(FD);

        if (FD->isTemplateInstantiation())
            return;

        if (FD->getTemplatedKind() != FunctionDecl::TK_NonTemplate)
            return;

        auto functionText = stringutils::rangetoStr(*(Result.SourceManager), FD->getSourceRange());

        std::string templateParam = "template<typename UnusedType>\n";
        functionText.insert(0, templateParam);

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), functionText);
    }
}

void MutatorFrontendAction_143::MutatorASTConsumer_143::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(unless(isExpansionInSystemHeader())).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}