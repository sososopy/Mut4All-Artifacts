//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Struct_Member_Type_With_Semicolon_193
 */ 
class MutatorFrontendAction_193 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(193)

private:
    class MutatorASTConsumer_193 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_193(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Struct_Member_Type_With_Semicolon_193.h"

// ========================================================================================================
#define MUT193_OUTPUT 1

void MutatorFrontendAction_193::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FieldDecl>("field")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FD->getLocation()))
            return;

        auto TSI = FD->getTypeSourceInfo();
        if (!TSI)
            return;

        auto TypeLoc = TSI->getTypeLoc();
        auto TypeRange = TypeLoc.getSourceRange();

        if (TypeRange.isInvalid())
            return;

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(TypeRange), ";");
    }
}

void MutatorFrontendAction_193::MutatorASTConsumer_193::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = fieldDecl().bind("field");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}