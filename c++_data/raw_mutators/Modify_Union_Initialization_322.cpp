//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Union_Initialization_322
 */ 
class MutatorFrontendAction_322 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(322)

private:
    class MutatorASTConsumer_322 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_322(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Modify_Union_Initialization_322.h"

// ========================================================================================================
#define MUT322_OUTPUT 1

void MutatorFrontendAction_322::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *UD = Result.Nodes.getNodeAs<clang::VarDecl>("UnionDecl")) {
        if (!UD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       UD->getLocation()))
            return;

        const auto *UT = UD->getType()->getAsUnionType();
        if (!UT)
            return;

        const auto *UnionDecl = UT->getDecl();
        for (const auto *Field : UnionDecl->fields()) {
            if (Field->getType()->isArrayType()) {
                auto InitRange = UD->getInit()->getSourceRange();
                std::string NewInit = "{ 12345 }";
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(InitRange), NewInit);
                break;
            }
        }
    }
}

void MutatorFrontendAction_322::MutatorASTConsumer_322::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = varDecl(hasType(unionType(has(fieldDecl(hasType(arrayType())))))).bind("UnionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}