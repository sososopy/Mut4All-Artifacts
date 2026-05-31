//source file
#include "../include/insert_unnamed_namespace_46.h"

// ========================================================================================================
#define MUT46_OUTPUT 1

void MutatorFrontendAction_46::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
            return;

        if (FD->hasBody()) {
            auto functionText = stringutils::rangetoStr(*(Result.SourceManager),
                                                        FD->getSourceRange());

            std::string unnamedNamespace = "namespace {\n/*mut46*/\n";
            unnamedNamespace += functionText;
            unnamedNamespace += "\n}\n";

            Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), unnamedNamespace);
        }
    }
}

void MutatorFrontendAction_46::MutatorASTConsumer_46::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isDefinition()).bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}