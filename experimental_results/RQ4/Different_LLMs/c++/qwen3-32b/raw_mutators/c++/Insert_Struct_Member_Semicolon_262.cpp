//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Struct_Member_Semicolon_262
 */ 
class MutatorFrontendAction_262 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(262)

private:
    class MutatorASTConsumer_262 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_262(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Insert_Struct_Member_Semicolon_262.h"

// ========================================================================================================
#define MUT262_OUTPUT 1

void MutatorFrontendAction_262::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *structDecl = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("structDecl")) {
        if (!structDecl || !Result.Context->getSourceManager().isWrittenInMainFile(structDecl->getLocation()))
            return;
        
        if (!structDecl->isStruct() || structDecl->isLambda())
            return;
        
        if (!structDecl->isCompleteDefinition())
            return;
        
        auto fields = structDecl->fields();
        if (fields.empty())
            return;
        auto firstField = *fields.begin();
        
        Rewrite.InsertText(firstField->getEndLoc(), ";");
    }
}

void MutatorFrontendAction_262::MutatorASTConsumer_262::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(isStruct(), has(fieldCountAtLeast(2))).bind("structDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}