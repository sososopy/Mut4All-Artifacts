//source file
#include "../include/Mutator_514.h"

// ========================================================================================================
#define MUT514_OUTPUT 1

void MutatorFrontendAction_514::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *NS = Result.Nodes.getNodeAs<clang::NamespaceDecl>("Namespace")) {
        if (!NS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     NS->getLocation()))
            return;

        for (auto *D : NS->decls()) {
            if (auto *TD = llvm::dyn_cast<clang::ClassTemplateDecl>(D)) {
                if (auto *RD = TD->getTemplatedDecl()) {
                    std::string templateName = RD->getNameAsString();
                    std::string deductionGuide = "template <class T> " + templateName + "() -> " + templateName + "<T>;";
                    SourceLocation insertLoc = NS->getEndLoc().getLocWithOffset(1);
                    Rewrite.InsertText(insertLoc, "\n/*mut514*/" + deductionGuide + "\n", true, true);
                }
            }
        }
    }
}

void MutatorFrontendAction_514::MutatorASTConsumer_514::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = namespaceDecl(has(classTemplateDecl())).bind("Namespace");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}