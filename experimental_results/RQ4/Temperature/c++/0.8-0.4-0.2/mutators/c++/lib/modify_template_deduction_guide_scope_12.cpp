//source file
#include "../include/Modify_Template_Deduction_Guide_Scope_12.h"

// ========================================================================================================
#define MUT12_OUTPUT 1

void MutatorFrontendAction_12::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DG = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("DeductionGuide")) {
        if (!DG || !Result.Context->getSourceManager().isWrittenInMainFile(DG->getLocation()))
            return;

        auto *NS = Result.Nodes.getNodeAs<clang::NamespaceDecl>("Namespace");
        if (!NS)
            return;

        auto deductionGuideText = stringutils::rangetoStr(*(Result.SourceManager), DG->getSourceRange());
        deductionGuideText = "\n/*mut12*/" + deductionGuideText;

        SourceLocation insertLoc = NS->getRBraceLoc().getLocWithOffset(-1);
        Rewrite.InsertText(insertLoc, deductionGuideText);
        Rewrite.RemoveText(DG->getSourceRange());
    }
}

void MutatorFrontendAction_12::MutatorASTConsumer_12::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto namespaceMatcher = namespaceDecl(has(namedDecl(hasDescendant(functionTemplateDecl())))).bind("Namespace");
    auto deductionGuideMatcher = functionTemplateDecl(hasAncestor(namespaceDecl())).bind("DeductionGuide");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(namespaceMatcher, &callback);
    matchFinder.addMatcher(deductionGuideMatcher, &callback);
    matchFinder.matchAST(Context);
}