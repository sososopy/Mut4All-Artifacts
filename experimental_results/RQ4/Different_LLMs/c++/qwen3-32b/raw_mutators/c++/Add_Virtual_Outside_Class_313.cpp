//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Virtual_Outside_Class_313
 */ 
class MutatorFrontendAction_313 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(313)

private:
    class MutatorASTConsumer_313 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_313(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Add_Virtual_Outside_Class_313.h"

// ========================================================================================================
#define MUT313_OUTPUT 1

void MutatorFrontendAction_313::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("methodDecl")) {
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(MT->getLocation()))
            return;
        if (!MT->isOutOfLine())
            return;
        SourceRange returnTypeRange = MT->getReturnTypeSourceRange();
        if (returnTypeRange.isInvalid())
            return;
        Rewrite.InsertTextBefore(returnTypeRange.getBegin(), "virtual ");
    }
}

void MutatorFrontendAction_313::MutatorASTConsumer_313::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxMethodDecl(hasBody(anything())).bind("methodDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}