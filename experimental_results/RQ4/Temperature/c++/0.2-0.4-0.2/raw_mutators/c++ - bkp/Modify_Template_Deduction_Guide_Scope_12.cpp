//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_template_deduction_guide_scope_12
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
        const NamespaceDecl* targetNamespace = nullptr;
    };
};

//source file
#include "../include/modify_template_deduction_guide_scope_12.h"

// ========================================================================================================
#define MUT12_OUTPUT 1

void MutatorFrontendAction_12::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TDG = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("TemplateDeductionGuide")) {
        if (!TDG || !Result.Context->getSourceManager().isWrittenInMainFile(TDG->getLocation()))
            return;

        const NamespaceDecl* enclosingNamespace = nullptr;
        for (const DeclContext* DC = TDG->getDeclContext(); DC; DC = DC->getParent()) {
            if (const auto* NS = dyn_cast<NamespaceDecl>(DC)) {
                enclosingNamespace = NS;
                break;
            }
        }

        if (enclosingNamespace && targetNamespace && enclosingNamespace != targetNamespace) {
            // Move the deduction guide inside the correct namespace
            SourceLocation startLoc = TDG->getBeginLoc();
            SourceLocation endLoc = TDG->getEndLoc().getLocWithOffset(1);
            std::string deductionGuideCode = Lexer::getSourceText(CharSourceRange::getTokenRange(startLoc, endLoc), Result.Context->getSourceManager(), LangOptions(), 0);
            Rewrite.RemoveText(CharSourceRange::getTokenRange(startLoc, endLoc));
            Rewrite.InsertTextAfter(targetNamespace->getRBraceLoc().getLocWithOffset(-1), "\n" + deductionGuideCode);
        } else if (enclosingNamespace && !targetNamespace) {
            // Move the deduction guide outside the namespace to introduce an error
            SourceLocation startLoc = TDG->getBeginLoc();
            SourceLocation endLoc = TDG->getEndLoc().getLocWithOffset(1);
            std::string deductionGuideCode = Lexer::getSourceText(CharSourceRange::getTokenRange(startLoc, endLoc), Result.Context->getSourceManager(), LangOptions(), 0);
            Rewrite.RemoveText(CharSourceRange::getTokenRange(startLoc, endLoc));
            Rewrite.InsertTextAfter(enclosingNamespace->getRBraceLoc().getLocWithOffset(1), "\n" + deductionGuideCode);
        }
    } else if (auto *NS = Result.Nodes.getNodeAs<clang::NamespaceDecl>("Namespace")) {
        if (!NS || !Result.Context->getSourceManager().isWrittenInMainFile(NS->getLocation()))
            return;
        targetNamespace = NS;
    }
}

void MutatorFrontendAction_12::MutatorASTConsumer_12::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher templateDeductionGuideMatcher = functionTemplateDecl(isTemplateDeductionGuide()).bind("TemplateDeductionGuide");
    DeclarationMatcher namespaceMatcher = namespaceDecl().bind("Namespace");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(templateDeductionGuideMatcher, &callback);
    matchFinder.addMatcher(namespaceMatcher, &callback);
    matchFinder.matchAST(Context);
}