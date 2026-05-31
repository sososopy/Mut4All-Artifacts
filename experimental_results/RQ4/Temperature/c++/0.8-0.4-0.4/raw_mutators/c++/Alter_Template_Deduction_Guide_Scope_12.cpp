//header file
#pragma once
#include "Mutator_base.h"

/**
 * alter_template_deduction_guide_scope_12
 */ 
class MutatorFrontendAction_12 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(12)

private:
    class MutatorASTConsumer_12 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_12(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result);
    private:
        Rewriter &Rewrite;
        const NamespaceDecl *originalNamespace = nullptr;
    };
};

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