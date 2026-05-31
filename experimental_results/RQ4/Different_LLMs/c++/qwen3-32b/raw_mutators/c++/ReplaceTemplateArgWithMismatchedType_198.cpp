//header file
#pragma once
#include "Mutator_base.h"

/**
 * ReplaceTemplateArgWithMismatchedType_198
 */ 
class MutatorFrontendAction_198 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(198)

private:
    class MutatorASTConsumer_198 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_198(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/ReplaceTemplateArgWithMismatchedType_198.h"

// ========================================================================================================
#define MUT198_OUTPUT 1

void MutatorFrontendAction_198::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("callExpr")) {
        // Filter nodes in header files
        if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                       CE->getBeginLoc()))
            return;

        if (CE->getNumTemplateArgs() < 2)
            return;

        // Get the second template argument's source range
        const clang::TemplateArgumentLoc &secondArg = CE->getTemplateArg(1);
        clang::SourceRange secondArgRange = secondArg.getSourceRange();

        // Replace the second template argument with 'int'
        Rewrite.ReplaceText(secondArgRange, "int");
    }
}

void MutatorFrontendAction_198::MutatorASTConsumer_198::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = callExpr(hasType(templateSpecializationType())).bind("callExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}