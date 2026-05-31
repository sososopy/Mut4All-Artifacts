//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Default_Argument_To_Generic_Lambda_Parameter_426
 */ 
class MutatorFrontendAction_426 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(426)
private:
    class MutatorASTConsumer_426 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_426(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Add_Default_Argument_To_Generic_Lambda_Parameter_426.h"

// ========================================================================================================
#define MUT426_OUTPUT 1

void MutatorFrontendAction_426::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *lambda = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambda")) {
        if (!lambda || !Result.Context->getSourceManager().isWrittenInMainFile(lambda->getLocation()))
            return;

        if (auto *param = Result.Nodes.getNodeAs<clang::ParmVarDecl>("param")) {
            if (!param || !Result.Context->getSourceManager().isWrittenInMainFile(param->getLocation()))
                return;

            SourceLocation nameLoc = param->getLocation();
            Rewrite.InsertTextAfter(nameLoc, " = 0");
        }
    }
}
  
void MutatorFrontendAction_426::MutatorASTConsumer_426::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = lambdaExpr(
        hasAnyParameter(
            parmVarDecl(
                hasType(autoType()),
                unless(hasDefaultArg())
            ).bind("param")
        )
    ).bind("lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}