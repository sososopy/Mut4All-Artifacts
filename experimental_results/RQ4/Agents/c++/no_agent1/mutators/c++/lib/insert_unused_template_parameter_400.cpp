//source file
#include "../include/insert_unused_template_parameter_400.h"

// ========================================================================================================
#define MUT400_OUTPUT 1

void MutatorFrontendAction_400::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
            return;

        if (FD->isTemplateInstantiation() || FD->isFunctionTemplateSpecialization())
            return;

        if (FD->getTemplatedKind() == clang::FunctionDecl::TK_NonTemplate) {
            std::string funcDecl = stringutils::rangetoStr(*(Result.SourceManager),
                                                           FD->getSourceRange());

            std::string templateParam = "template<typename UnusedType>\n";
            std::string newFuncDecl = templateParam + funcDecl;

            Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), newFuncDecl);
        }
    }
}

void MutatorFrontendAction_400::MutatorASTConsumer_400::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(unless(isTemplateInstantiation())).bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}