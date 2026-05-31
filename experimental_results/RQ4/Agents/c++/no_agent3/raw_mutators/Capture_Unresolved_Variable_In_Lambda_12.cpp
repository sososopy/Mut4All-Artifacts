//header file
#pragma once
#include "Mutator_base.h"

/**
 * Capture_Unresolved_Variable_In_Lambda_12
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
#include "../include/Capture_Unresolved_Variable_In_Lambda_12.h"

// ========================================================================================================
#define MUT12_OUTPUT 1

void MutatorFrontendAction_12::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("lambdaVar")) {
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       VD->getLocation()))
            return;

        if (auto *LambdaExpr = dyn_cast<clang::LambdaExpr>(VD->getInit()->IgnoreImplicit())) {
            auto lambdaRange = LambdaExpr->getSourceRange();
            auto lambdaText = stringutils::rangetoStr(*(Result.SourceManager), lambdaRange);

            std::string captureInsert = "[captured_var]";
            size_t pos = lambdaText.find('[');
            if (pos != std::string::npos) {
                lambdaText.insert(pos + 1, captureInsert);
            } else {
                pos = lambdaText.find('(');
                if (pos != std::string::npos) {
                    lambdaText.insert(pos, captureInsert);
                }
            }

            std::string varDeclaration = "int captured_var = 42;\n";
            lambdaText += varDeclaration;

            Rewrite.ReplaceText(CharSourceRange::getTokenRange(lambdaRange), lambdaText);
        }
    }
}

void MutatorFrontendAction_12::MutatorASTConsumer_12::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = varDecl(hasInitializer(lambdaExpr())).bind("lambdaVar");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}