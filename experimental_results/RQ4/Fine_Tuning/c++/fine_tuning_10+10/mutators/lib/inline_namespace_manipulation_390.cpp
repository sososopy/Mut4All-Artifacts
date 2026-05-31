//source file
#include "../include/Inline_Namespace_Manipulation_390.h"

// ========================================================================================================
#define MUT390_OUTPUT 1

void MutatorFrontendAction_390::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *NS = Result.Nodes.getNodeAs<clang::NamespaceDecl>("InlineNamespace")) {
        if (!NS || !Result.Context->getSourceManager().isWrittenInMainFile(NS->getLocation()))
            return;

        for (auto *D : NS->decls()) {
            if (auto *CD = llvm::dyn_cast<clang::CXXRecordDecl>(D)) {
                std::string originalText = stringutils::rangetoStr(*(Result.SourceManager), CD->getSourceRange());
                std::string mutatedText = "inline namespace Z { " + originalText + " }";
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(CD->getSourceRange()), mutatedText);
            } else if (auto *FD = llvm::dyn_cast<clang::FunctionDecl>(D)) {
                std::string originalText = stringutils::rangetoStr(*(Result.SourceManager), FD->getSourceRange());
                std::string mutatedText = "inline namespace Z { " + originalText + " }";
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), mutatedText);
            }
        }
    }
}

void MutatorFrontendAction_390::MutatorASTConsumer_390::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = namespaceDecl(isInline()).bind("InlineNamespace");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}