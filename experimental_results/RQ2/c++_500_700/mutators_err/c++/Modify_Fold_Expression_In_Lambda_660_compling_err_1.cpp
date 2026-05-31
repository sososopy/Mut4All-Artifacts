//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_fold_expression_in_lambda_660
 */ 
class MutatorFrontendAction_660 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(660)

private:
    class MutatorASTConsumer_660 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_660(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_fold_expression_in_lambda_660.h"

// ========================================================================================================
#define MUT660_OUTPUT 1

void MutatorFrontendAction_660::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LambdaExpr = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambdaExpr")) {
        if (!LambdaExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                LambdaExpr->getBeginLoc()))
            return;

        auto LambdaBody = LambdaExpr->getBody();
        if (!LambdaBody)
            return;

        for (auto &Stmt : LambdaBody->children()) {
            if (auto *FoldExpr = llvm::dyn_cast<clang::BinaryOperator>(Stmt)) {
                if (FoldExpr->isFold()) {
                    std::string OriginalText = Lexer::getSourceText(
                        CharSourceRange::getTokenRange(FoldExpr->getSourceRange()),
                        Rewrite.getSourceMgr(), Rewrite.getLangOpts()).str();

                    std::string MutatedText = OriginalText;
                    if (FoldExpr->getOpcode() == clang::BO_Add) {
                        llvm::outs() << "Original Fold Expression: " << OriginalText << "\n";
                        MutatedText = std::regex_replace(MutatedText, std::regex("\\+"), "*");
                        llvm::outs() << "Mutated Fold Expression: " << MutatedText << "\n";
                    }

                    Rewrite.ReplaceText(FoldExpr->getSourceRange(), MutatedText);
                }
            }
        }
    }
}
  
void MutatorFrontendAction_660::MutatorASTConsumer_660::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = lambdaExpr(hasDescendant(binaryOperator(isFoldExpression()))).bind("lambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}