//source file
#include "../include/invalid_template_deduction_guide_12.h"

// ========================================================================================================
#define MUT12_OUTPUT 1

void MutatorFrontendAction_12::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *NSD = Result.Nodes.getNodeAs<clang::NamespaceDecl>("Namespace")) {
        if (!NSD || !Result.Context->getSourceManager().isWrittenInMainFile(NSD->getLocation()))
            return;

        for (auto *D : NSD->decls()) {
            if (auto *TSD = llvm::dyn_cast<clang::ClassTemplateDecl>(D)) {
                std::string templateName = TSD->getNameAsString();
                std::string namespaceName = NSD->getNameAsString();
                if (namespaces.find(namespaceName) == namespaces.end()) {
                    namespaces.insert(namespaceName);
                    std::string deductionGuide = "template <typename U> " + templateName + "() -> " + templateName + "<U>;";
                    SourceLocation insertLoc = NSD->getEndLoc().getLocWithOffset(1);
                    Rewrite.InsertText(insertLoc, "\n/*mut12*/" + deductionGuide + "\n");
                }
            }
        }
    }
}

void MutatorFrontendAction_12::MutatorASTConsumer_12::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = namespaceDecl(hasDescendant(classTemplateDecl())).bind("Namespace");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}