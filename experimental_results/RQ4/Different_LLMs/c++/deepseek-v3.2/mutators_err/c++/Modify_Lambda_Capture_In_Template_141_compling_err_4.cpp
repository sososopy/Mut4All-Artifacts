//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Lambda_Capture_In_Template_141
 */ 
class MutatorFrontendAction_141 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(141)

private:
    class MutatorASTConsumer_141 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_141(Rewriter &R) : TheRewriter(R) {}
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
#include "mutator_1.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/Transformer/RangeSelector.h"
#include "clang/Tooling/Transformer/Stencil.h"
#include "clang/Tooling/Transformer/Transformer.h"
#include "llvm/Support/raw_ostream.h"

using namespace clang;
using namespace clang::ast_matchers;

void MutatorFrontendAction_141::MutatorASTConsumer_141::HandleTranslationUnit(ASTContext &Context) {
    TheRewriter.setSourceMgr(Context.getSourceManager(), Context.getLangOpts());
    MatchFinder Finder;
    Callback TheCallback(TheRewriter);
    Finder.addMatcher(lambdaExpr().bind("lambda"), &TheCallback);
    Finder.matchAST(Context);
}

void MutatorFrontendAction_141::Callback::run(const MatchFinder::MatchResult &Result) {
    if (const auto *LE = Result.Nodes.getNodeAs<LambdaExpr>("lambda")) {
        SourceManager &SM = *Result.SourceManager;
        LangOptions LangOpts = Result.Context->getLangOpts();
        Rewrite.setSourceMgr(SM, LangOpts);
        
        auto CaptureDefault = LE->getCaptureDefault();
        if (CaptureDefault == LCD_None) {
            SourceLocation CaptureStart = LE->getIntroducerRange().getBegin();
            Rewrite.InsertText(CaptureStart, "=, ", true, true);
        }
    }
}