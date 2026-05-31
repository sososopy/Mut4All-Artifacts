//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Lambda_Capture_By_Value_75
 */ 
class MutatorFrontendAction_75 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(75)

private:
    class MutatorASTConsumer_75 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_75(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::LambdaExpr*> lambdaExprs;
    };
};

//source file
#include "../include/Replace_Lambda_Capture_By_Value_75.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"

using namespace clang;
using namespace clang::ast_matchers;
using namespace clang::tooling;
using namespace llvm;

void MutatorFrontendAction_75::MutatorASTConsumer_75::HandleTranslationUnit(ASTContext &Context) {
    TheRewriter.setSourceMgr(Context.getSourceManager(), Context.getLangOpts());
    MatchFinder Finder;
    Callback callback(TheRewriter);
    Finder.addMatcher(lambdaExpr().bind("lambda"), &callback);
    Finder.matchAST(Context);
}

void MutatorFrontendAction_75::Callback::run(const MatchFinder::MatchResult &Result) {
    const LambdaExpr *lambda = Result.Nodes.getNodeAs<LambdaExpr>("lambda");
    if (!lambda) return;
    lambdaExprs.push_back(lambda);
    if (lambdaExprs.size() >= 2) {
        const LambdaExpr *first = lambdaExprs[0];
        const LambdaExpr *second = lambdaExprs[1];
        auto firstCaptures = first->captures();
        auto secondCaptures = second->captures();
        if (!firstCaptures.empty() && !secondCaptures.empty()) {
            auto firstCapture = firstCaptures[0];
            auto secondCapture = secondCaptures[0];
            if (firstCapture.getCaptureKind() == LambdaCaptureKind::LCK_ByCopy &&
                secondCapture.getCaptureKind() == LambdaCaptureKind::LCK_ByRef) {
                SourceRange firstRange = firstCapture.getLocation();
                SourceRange secondRange = secondCapture.getLocation();
                std::string firstText = Rewrite.getRewrittenText(firstRange);
                std::string secondText = Rewrite.getRewrittenText(secondRange);
                Rewrite.ReplaceText(firstRange, secondText);
                Rewrite.ReplaceText(secondRange, firstText);
            }
        }
        lambdaExprs.clear();
    }
}