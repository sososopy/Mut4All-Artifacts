//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Constraint_Violation_In_Lambda_61
 */ 
class MutatorFrontendAction_61 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(61)

private:
    class MutatorASTConsumer_61 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_61(Rewriter &R) : TheRewriter(R) {}
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
        const FunctionDecl *CurrentFunction = nullptr;
    };
};

//source file
#include "../include/template_constraint_violation_in_lambda_61.h"

// ========================================================================================================
#define MUT61_OUTPUT 1

void MutatorFrontendAction_61::Callback::run(const MatchFinder::MatchResult &Result) {
    if (const auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("templateFunction")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        CurrentFunction = FD;

        for (const auto *Body : FD->getBody()->children()) {
            if (const auto *LambdaExpr = dyn_cast<LambdaExpr>(Body)) {
                const auto *CallOperator = LambdaExpr->getCallOperator();
                if (CallOperator && CallOperator->getNumParams() == 1) {
                    const auto *ParamType = CallOperator->getParamDecl(0)->getType().getTypePtr();
                    
                    std::string OriginalTypeStr = ParamType->getCanonicalTypeInternal().getAsString();
                    std::string MutatedTypeStr = "unsigned int";

                    if (OriginalTypeStr != MutatedTypeStr) {
                        std::string LambdaCall = "/*mut61*/" + FD->getNameAsString() + "(" + OriginalTypeStr + "())(" + MutatedTypeStr + "(0));";
                        SourceLocation InsertLoc = FD->getBodyRBrace();
                        Rewrite.InsertTextBefore(InsertLoc, LambdaCall);
                    }
                }
            }
        }
    }
}

void MutatorFrontendAction_61::MutatorASTConsumer_61::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isTemplateInstantiation(), hasDescendant(lambdaExpr())).bind("templateFunction");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}