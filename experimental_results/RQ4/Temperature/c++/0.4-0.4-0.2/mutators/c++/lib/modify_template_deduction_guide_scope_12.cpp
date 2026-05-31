//source file
#include "../include/modify_template_deduction_guide_scope_12.h"

// ========================================================================================================
#define MUT12_OUTPUT 1

void MutatorFrontendAction_12::Callback::run(const MatchFinder::MatchResult &Result) {
    // Check whether the matched AST node is the target node
    if (auto *TDG = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("TemplateDeductionGuide")) {
        if (!TDG || !Result.Context->getSourceManager().isWrittenInMainFile(TDG->getLocation()))
            return;

        if (auto *NS = Result.Nodes.getNodeAs<clang::NamespaceDecl>("AssociatedNamespace")) {
            AssociatedNamespace = NS;
        }

        if (AssociatedNamespace) {
            // Get the source code text of target node
            auto deductionGuideText = stringutils::rangetoStr(*(Result.SourceManager), TDG->getSourceRange());
            // Perform mutation on the source code text by applying string replacement
            std::string insertionText = deductionGuideText + "\n";
            // Replace the original AST node with the mutated one
            Rewrite.InsertTextAfterToken(AssociatedNamespace->getRBraceLoc(), insertionText);
            Rewrite.RemoveText(TDG->getSourceRange());
        }
    }
}

void MutatorFrontendAction_12::MutatorASTConsumer_12::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define ASTMatchers to identify the target AST node for mutation
    auto templateDeductionGuideMatcher = functionTemplateDecl().bind("TemplateDeductionGuide");
    auto associatedNamespaceMatcher = namespaceDecl(hasDescendant(functionTemplateDecl())).bind("AssociatedNamespace");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(templateDeductionGuideMatcher, &callback);
    matchFinder.addMatcher(associatedNamespaceMatcher, &callback);
    matchFinder.matchAST(Context);
}