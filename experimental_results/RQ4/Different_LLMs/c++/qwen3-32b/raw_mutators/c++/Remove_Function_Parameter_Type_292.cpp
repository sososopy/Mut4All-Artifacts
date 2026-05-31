//header file
#pragma once
#include "Mutator_base.h"

/**
 * Remove_Function_Parameter_Type_292
 */ 
class MutatorFrontendAction_292 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(292)

private:
    class MutatorASTConsumer_292 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_292(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Remove_Function_Parameter_Type_292.h"

// ========================================================================================================
#define MUT292_OUTPUT 1

void MutatorFrontendAction_292::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *param = Result.Nodes.getNodeAs<clang::ParmVarDecl>("param")) {
        // Filter nodes in header files
        if (!param || !Result.Context->getSourceManager().isWrittenInMainFile(
                       param->getLocation()))
            return;
        // Get the source range of the type
        auto typeLoc = param->getTypeSourceInfo()->getTypeLoc();
        SourceRange typeRange = typeLoc.getSourceRange();
        // Remove the type from the source code
        Rewrite.RemoveText(typeRange);
    }
}

void MutatorFrontendAction_292::MutatorASTConsumer_292::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define the AST matcher for function parameters with explicit types
    DeclarationMatcher matcher = parmVarDecl(hasType(), isParameter()).bind("param");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}