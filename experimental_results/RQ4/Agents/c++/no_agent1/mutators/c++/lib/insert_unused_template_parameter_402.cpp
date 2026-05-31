//source file
#include "../include/insert_unused_template_parameter_402.h"

// ========================================================================================================
#define MUT402_OUTPUT 1

void MutatorFrontendAction_402::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FD->getLocation()))
            return;

        if (FD->isTemplateInstantiation() || FD->isDependentContext())
            return;

        auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   FD->getSourceRange());

        std::string templateParam = "template<typename T = int> ";
        declaration.insert(0, templateParam);

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), declaration);
    }
}

void MutatorFrontendAction_402::MutatorASTConsumer_402::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isDefinition()).bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}