//header file
#pragma once
#include "Mutator_base.h"

/**
 * InvalidAnonymousNamespaceInFunction_363
 */ 
class MutatorFrontendAction_363 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(363)

private:
    class MutatorASTConsumer_363 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_363(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/InvalidAnonymousNamespaceInFunction_363.h"

// ========================================================================================================
#define MUT363_OUTPUT 1

void MutatorFrontendAction_363::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
        // Filter nodes in header files
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(MT->getLocation()))
            return;

        if (!MT->hasBody())
            return;

        auto body = MT->getBody();
        if (!body)
            return;

        // Get the location after the opening brace of the body
        SourceLocation insertLoc = body->getLBraceLoc().getLocWithOffset(1);
        // Insert the anonymous namespace
        Rewrite.InsertText(insertLoc, "namespace { int y = 0; }\n", true, true);
    }
}

void MutatorFrontendAction_363::MutatorASTConsumer_363::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define ASTMatchers to identify the target AST node for mutation
    DeclarationMatcher matcher = functionDecl(isDefinition()).bind("Function") ||
                                   cxxMethodDecl(isDefinition()).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}