//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Nested_Forwarding_Helper_397
 */ 
class MutatorFrontendAction_397 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(397)

private:
    class MutatorASTConsumer_397 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_397(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        bool helperInserted = false;
    };
};

//source file
#include "../include/Add_Nested_Forwarding_Helper_397.h"

// ========================================================================================================
#define MUT397_OUTPUT 1

void MutatorFrontendAction_397::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("forwardCall")) {
        if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(CE->getBeginLoc()))
            return;

        QualType calleeType = CE->getCallee()->getType();
        if (auto *TST = calleeType->getAs<clang::TemplateSpecializationType>()) {
            clang::TemplateArgumentList *TemplateArgs = TST->getArgs();
            if (!TemplateArgs || TemplateArgs->size() < 1)
                return;

            clang::TemplateArgument TA = TemplateArgs->get(0);
            clang::PrintingPolicy Policy = Result.Context->getPrintingPolicy();
            std::string TAText;
            llvm::raw_string_ostream OS(TAText);
            TA.print(Policy, OS, false);
            OS.flush();

            std::string newCode = "helper_forward<" + TAText + ">(std::forward<" + TAText + ">())";
            Rewrite.ReplaceText(CE->getSourceRange(), newCode);

            if (!helperInserted) {
                std::string helperCode =
                    "template<class T>\n"
                    "constexpr T&& helper_forward(T&& arg) { return static_cast<T&&>(arg); }\n";
                SourceLocation startLoc = Result.SourceManager->getLocForStartOfFile(
                    Result.SourceManager->getMainFileID());
                Rewrite.InsertText(startLoc, helperCode, true, true);
                helperInserted = true;
            }
        }
    }
}
  
void MutatorFrontendAction_397::MutatorASTConsumer_397::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    StatementMatcher matcher = callExpr(callee(functionDecl(hasName("forward"), hasParent(namespaceDecl(hasName("std")))))).bind("forwardCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}