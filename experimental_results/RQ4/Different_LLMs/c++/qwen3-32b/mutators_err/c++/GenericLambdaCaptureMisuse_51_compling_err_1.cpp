//header file
#pragma once
#include "Mutator_base.h"

/**
 * GenericLambdaCaptureMisuse_51
 */ 
class MutatorFrontendAction_51 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(51)

private:
    class MutatorASTConsumer_51 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_51(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/GenericLambdaCaptureMisuse_51.h"

// ========================================================================================================
#define MUT51_OUTPUT 1

void MutatorFrontendAction_51::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *lambda = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambda")) {
        if (!lambda || !Result.Context->getSourceManager().isWrittenInMainFile(lambda->getBeginLoc()))
            return;

        CXXMethodDecl *callOperator = lambda->getCallOperator();
        if (!callOperator)
            return;

        bool isGeneric = true;
        for (auto param : callOperator->parameters()) {
            if (!param->getType().getTypePtr()->isAutoType()) {
                isGeneric = false;
                break;
            }
        }

        if (!isGeneric) {
            for (auto param : callOperator->parameters()) {
                if (param->getType().getTypePtr()->isAutoType())
                    continue;

                SourceLocation typeLoc = param->getTypeSourceInfo()->getTypeLoc().getBeginLoc();
                SourceRange typeRange = param->getTypeSourceInfo()->getTypeLoc().getSourceRange();
                if (typeLoc.isValid() && typeRange.isValid()) {
                    Rewrite.ReplaceText(typeRange, "auto");
                }
            }
        }
    }
}
  
void MutatorFrontendAction_51::MutatorASTConsumer_51::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl(hasDescendant(lambdaExpr())).bind("lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}