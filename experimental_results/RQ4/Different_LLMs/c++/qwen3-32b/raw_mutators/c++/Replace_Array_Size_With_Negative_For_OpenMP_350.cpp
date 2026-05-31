//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Array_Size_With_Negative_For_OpenMP_350
 */ 
class MutatorFrontendAction_350 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(350)

private:
    class MutatorASTConsumer_350 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_350(Rewriter &R) : TheRewriter(R) {}
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
        // Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Replace_Array_Size_With_Negative_For_OpenMP_350.h"

// ========================================================================================================
#define MUT350_OUTPUT 1

void MutatorFrontendAction_350::Callback::run(const MatchFinder::MatchResult &Result) {
    // Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("arrayDecl")) {
        // Filter nodes in header files
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(VD->getLocation()))
            return;

        // Get the type
        QualType T = VD->getType();
        if (const auto *AT = T->getAs<ConstantArrayType>()) {
            // Check if the array has a constant size
            if (AT->getSize().isKnown()) {
                // Get the type source info
                const TypeSourceInfo *TSI = VD->getTypeSourceInfo();
                if (!TSI)
                    return;

                TypeLoc TL = TSI->getTypeLoc();
                if (const auto *ArrayTL = TL.getAs<ArrayTypeLoc>()) {
                    // Get the size expression
                    Expr *SizeExpr = ArrayTL->getSizeExpr();
                    if (SizeExpr) {
                        // Replace the size expression with -1
                        SourceRange SizeRange = SizeExpr->getSourceRange();
                        Rewrite.ReplaceText(SizeRange, "-1");
                    }
                }
            }
        }
    }
}

void MutatorFrontendAction_350::MutatorASTConsumer_350::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = varDecl(hasType(arrayType())).bind("arrayDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}