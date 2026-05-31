//source file
#include "../include/alter_template_deduction_guide_scope_12.h"

// ========================================================================================================
#define MUT12_OUTPUT 1

void MutatorFrontendAction_12::Callback::run(const MatchFinder::MatchResult &Result) {
    // Check whether the matched AST node is the target node
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateDecl")) {
        // Filter nodes in header files
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;
        
        originalNamespace = dyn_cast<NamespaceDecl>(TD->getDeclContext());
    } else if (auto *DG = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("DeductionGuide")) {
        if (!DG || !Result.Context->getSourceManager().isWrittenInMainFile(DG->getLocation()))
            return;
        
        if (originalNamespace) {
            std::string qualifiedName = originalNamespace->getNameAsString() + "::" + DG->getNameAsString();
            std::string newNamespace = "namespace MutatedNamespace {\n";
            std::string guideText = "template <typename T> " + qualifiedName + "() -> " + qualifiedName + "<T>;\n";
            std::string closeNamespace = "}\n";
            
            SourceLocation startLoc = DG->getBeginLoc();
            SourceLocation endLoc = DG->getEndLoc().getLocWithOffset(1);
            
            Rewrite.ReplaceText(SourceRange(startLoc, endLoc), newNamespace + guideText + closeNamespace);
        }
    }
}

void MutatorFrontendAction_12::MutatorASTConsumer_12::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher templateMatcher = classTemplateDecl().bind("TemplateDecl");
    DeclarationMatcher deductionGuideMatcher = functionTemplateDecl().bind("DeductionGuide");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(templateMatcher, &callback);
    matchFinder.addMatcher(deductionGuideMatcher, &callback);
    matchFinder.matchAST(Context);
}