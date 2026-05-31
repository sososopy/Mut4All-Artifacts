//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Lambda_Expression_With_Pack_Expansion_460
 */ 
class MutatorFrontendAction_460 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(460)

private:
    class MutatorASTConsumer_460 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_460(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_lambda_expression_with_pack_expansion_460.h"

// ========================================================================================================
#define MUT460_OUTPUT 1

void MutatorFrontendAction_460::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("LambdaExpr")) {
        if (!LE || !Result.Context->getSourceManager().isWrittenInMainFile(
                       LE->getLocation()))
            return;

        auto body = LE->getBody();
        if (!body)
            return;

        for (auto &stmt : body->children()) {
            if (auto *FE = llvm::dyn_cast<clang::BinaryOperator>(stmt)) {
                if (FE->isFoldable()) {
                    auto op = FE->getOpcode();
                    std::string newOp;
                    switch (op) {
                        case clang::BO_Add:
                            newOp = "*";
                            break;
                        case clang::BO_Comma:
                            newOp = "+";
                            break;
                        default:
                            continue;
                    }
                    auto opLoc = FE->getOperatorLoc();
                    Rewrite.ReplaceText(opLoc, 1, newOp);
                }
            }
        }
    }
}
  
void MutatorFrontendAction_460::MutatorASTConsumer_460::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = lambdaExpr(hasDescendant(binaryOperator(isFoldable()).bind("FoldExpr"))).bind("LambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}