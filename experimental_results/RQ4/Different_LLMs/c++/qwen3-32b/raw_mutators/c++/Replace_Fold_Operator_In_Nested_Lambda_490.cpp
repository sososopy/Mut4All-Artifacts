//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Fold_Operator_In_Nested_Lambda_490
 */ 
class MutatorFrontendAction_490 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(490)

private:
    class MutatorASTConsumer_490 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_490(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Replace_Fold_Operator_In_Nested_Lambda_490.h"

// ========================================================================================================
#define MUT490_OUTPUT 1

void MutatorFrontendAction_490::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FoldExpr = Result.Nodes.getNodeAs<clang::CXXFoldExpr>("FoldExpr")) {
        if (!FoldExpr || !Result.Context->getSourceManager().isWrittenInMainFile(FoldExpr->getBeginLoc()))
            return;
        if (FoldExpr->getOperator() != clang::OO_Comma)
            return;

        SourceLocation OpLoc = FoldExpr->getOperatorLoc();
        SourceLocation EndLoc = OpLoc.getLocWithOffset(1);
        Rewrite.ReplaceText(SourceRange(OpLoc, EndLoc), "||");

        MatchFinder nestedFinder;
        class NestedLambdaCallback : public MatchFinder::MatchCallback {
        public:
            NestedLambdaCallback(Rewriter &R, SourceRange FoldRange) : Rewrite(R), FoldRange(FoldRange) {}
            void run(const MatchFinder::MatchResult &NestedResult) override {
                if (auto *Lambda = NestedResult.Nodes.getNodeAs<clang::LambdaExpr>("NestedLambda")) {
                    if (!clang::Lexer::isTokenInOriginalSource(Lambda->getBeginLoc(), NestedResult.SourceManager))
                        return;

                    const clang::Stmt *Body = Lambda->getBody();
                    if (!Body)
                        return;

                    SourceRange BodyRange = Body->getSourceRange();
                    std::string BodyText = stringutils::rangetoStr(*NestedResult.SourceManager, BodyRange);

                    std::string NewBody = "return static_cast<bool>(" + BodyText + ");";
                    Rewrite.ReplaceText(BodyRange, NewBody);
                }
            }
        private:
            Rewriter &Rewrite;
            SourceRange FoldRange;
        };

        NestedLambdaCallback nestedCb(Rewrite, FoldExpr->getSourceRange());
        nestedFinder.addMatcher(lambdaExpr().bind("NestedLambda"), &nestedCb);
        nestedFinder.matchAST(*Result.Context);
    }
}
  
void MutatorFrontendAction_490::MutatorASTConsumer_490::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxFoldExpr(hasOperatorName(",")).bind("FoldExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}