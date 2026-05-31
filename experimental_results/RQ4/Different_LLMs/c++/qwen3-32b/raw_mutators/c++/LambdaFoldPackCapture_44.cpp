//header file
#pragma once
#include "Mutator_base.h"

/**
 * LambdaFoldPackCapture_44
 */ 
class MutatorFrontendAction_44 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(44)

private:
    class MutatorASTConsumer_44 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_44(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/LambdaFoldPackCapture_44.h"

// ========================================================================================================
#define MUT44_OUTPUT 1

void MutatorFrontendAction_44::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXFoldExpr>("FoldExpr")) {
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(MT->getLocation()))
            return;

        auto opStr = MT->getOperator().getOpcodeStr();
        auto lhs = MT->getLHS();
        auto rhs = MT->getRHS();

        std::string lhsStr = stringutils::rangetoStr(*Result.SourceManager, lhs->getSourceRange());
        std::string rhsStr;
        if (rhs) {
            rhsStr = stringutils::rangetoStr(*Result.SourceManager, rhs->getSourceRange());
        } else {
            rhsStr = "0";
        }

        std::string lambda = "[" + lhsStr + "]{ return " + lhsStr + " " + opStr + " " + rhsStr + "; }()";
        std::string newExpr = "(" + lambda + " " + opStr + " ...)";

        Rewrite.ReplaceText(MT->getSourceRange(), newExpr);
    }
}

void MutatorFrontendAction_44::MutatorASTConsumer_44::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxFoldExpr().bind("FoldExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}