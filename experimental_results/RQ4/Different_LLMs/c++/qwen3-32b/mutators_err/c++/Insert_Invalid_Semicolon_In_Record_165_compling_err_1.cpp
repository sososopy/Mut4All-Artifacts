//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Invalid_Semicolon_In_Record_165
 */ 
class MutatorFrontendAction_165 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(165)

private:
    class MutatorASTConsumer_165 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_165(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Insert_Invalid_Semicolon_In_Record_165.h"

// ========================================================================================================
#define MUT165_OUTPUT 1

void MutatorFrontendAction_165::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *recordDecl = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("record")) {
        if (!recordDecl || !Result.Context->getSourceManager().isWrittenInMainFile(recordDecl->getLocation()))
            return;
        if (!recordDecl->isCompleteDefinition())
            return;
        if (!recordDecl->isStruct() && !recordDecl->isUnion() && !recordDecl->isClass())
            return;
        auto fields = recordDecl->fields();
        if (fields.size() < 2)
            return;
        auto firstField = *fields.begin();
        auto secondField = *std::next(fields.begin());
        Rewrite.InsertText(firstField->getEndLoc(), ";", true, true);
    }
}

void MutatorFrontendAction_165::MutatorASTConsumer_165::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl().bind("record");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}