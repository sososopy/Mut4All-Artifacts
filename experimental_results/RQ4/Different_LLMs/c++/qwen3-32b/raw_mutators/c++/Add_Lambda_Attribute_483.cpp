//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Lambda_Attribute_483
 */ 
class MutatorFrontendAction_483 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(483)
private:
    class MutatorASTConsumer_483 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_483(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Add_Lambda_Attribute_483.h"

// ========================================================================================================
#define MUT483_OUTPUT 1

void MutatorFrontendAction_483::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambda")) {
        if (!LE || !Result.Context->getSourceManager().isWrittenInMainFile(LE->getBeginLoc()))
            return;

        // Find the location to insert the attribute (after parameters, before body)
        SourceLocation bodyLoc = LE->getBody()->getBeginLoc();
        if (bodyLoc.isInvalid())
            return;

        // Insert pcs attribute
        Rewrite.InsertText(bodyLoc, " __attribute__((pcs(\"target\")))", true, true);
    }
}
  
void MutatorFrontendAction_483::MutatorASTConsumer_483::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = lambdaExpr(hasAncestor(functionTemplateDecl())).bind("lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}