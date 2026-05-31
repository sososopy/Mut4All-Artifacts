//header file
#pragma once
#include "Mutator_base.h"

/**
 * Pointer_Dereference_Mismatch_277
 */ 
class MutatorFrontendAction_277 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(277)

private:
    class MutatorASTConsumer_277 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_277(Rewriter &R) : TheRewriter(R) {}
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
        const VarDecl *nonPointerVar = nullptr; // Record a non-pointer variable for substitution
    };
};

//source file
#include "../include/Pointer_Dereference_Mismatch_277.h"

// ========================================================================================================
#define MUT277_OUTPUT 1

void MutatorFrontendAction_277::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FC = Result.Nodes.getNodeAs<clang::CallExpr>("funcCall")) {
        if (!FC || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FC->getBeginLoc()))
            return;

        for (unsigned i = 0; i < FC->getNumArgs(); ++i) {
            if (auto *DE = dyn_cast<clang::UnaryOperator>(FC->getArg(i))) {
                if (DE->getOpcode() == clang::UO_Deref) {
                    if (nonPointerVar) {
                        auto varName = nonPointerVar->getNameAsString();
                        auto argRange = DE->getSourceRange();
                        Rewrite.ReplaceText(CharSourceRange::getTokenRange(argRange), varName);
                        break;
                    }
                }
            }
        }
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("nonPointerVar")) {
        if (!VD->getType()->isPointerType() && !VD->getType()->isArrayType()) {
            nonPointerVar = VD;
        }
    }
}

void MutatorFrontendAction_277::MutatorASTConsumer_277::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto funcCallMatcher = callExpr(hasAnyArgument(unaryOperator(hasOperatorName("*")))).bind("funcCall");
    auto nonPointerVarMatcher = varDecl(unless(hasType(pointerType())), unless(hasType(arrayType()))).bind("nonPointerVar");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(funcCallMatcher, &callback);
    matchFinder.addMatcher(nonPointerVarMatcher, &callback);
    matchFinder.matchAST(Context);
}