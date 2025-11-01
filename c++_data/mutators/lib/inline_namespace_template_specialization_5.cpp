//source file
#include "../include/inline_namespace_template_specialization_5.h"

// ========================================================================================================
#define MUT5_OUTPUT 1

void MutatorFrontendAction_5::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *NS = Result.Nodes.getNodeAs<clang::NamespaceDecl>("inlineNamespace")) {
        if (!NS || !Result.Context->getSourceManager().isWrittenInMainFile(NS->getLocation()))
            return;

        for (auto *D : NS->decls()) {
            if (auto *TD = llvm::dyn_cast<clang::FunctionTemplateDecl>(D)) {
                if (TD->getTemplatedDecl()->isDefined()) {
                    auto funcName = TD->getNameAsString();
                    std::string specialization = "\n/*mut5*/template <> int " + funcName + "<int>(int);\n";
                    Rewrite.InsertTextAfterToken(NS->getEndLoc(), specialization);
                }
            }
        }
    }
}

void MutatorFrontendAction_5::MutatorASTConsumer_5::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = namespaceDecl(isInline()).bind("inlineNamespace");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}