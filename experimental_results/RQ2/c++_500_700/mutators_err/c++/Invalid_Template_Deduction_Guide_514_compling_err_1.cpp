//header file
#pragma once
#include "Mutator_base.h"

/**
 * invalid_template_deduction_guide_514
 */ 
class MutatorFrontendAction_514 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(514)

private:
    class MutatorASTConsumer_514 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_514(Rewriter &R) : TheRewriter(R) {}
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
    };
};

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
    DeclarationMatcher matcher = namespaceDecl(has(decl(anyOf(classTemplateDecl())))).bind("Namespace");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}