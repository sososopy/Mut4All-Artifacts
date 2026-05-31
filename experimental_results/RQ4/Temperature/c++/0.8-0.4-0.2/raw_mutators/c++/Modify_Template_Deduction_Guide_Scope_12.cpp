//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Deduction_Guide_Scope_12
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
    };
};

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
    auto namespaceMatcher = namespaceDecl(has(namedDecl(hasDescendant(templateDecl())))).bind("Namespace");
    auto deductionGuideMatcher = functionTemplateDecl(hasAncestor(namespaceDecl())).bind("DeductionGuide");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(namespaceMatcher, &callback);
    matchFinder.addMatcher(deductionGuideMatcher, &callback);
    matchFinder.matchAST(Context);
}