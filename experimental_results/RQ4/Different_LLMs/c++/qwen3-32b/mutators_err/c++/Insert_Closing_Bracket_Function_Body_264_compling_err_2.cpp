//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Closing_Bracket_Function_Body_264
 */ 
class MutatorFrontendAction_264 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(264)

private:
    class MutatorASTConsumer_264 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_264(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
    };
};

//source file
#include "../include/Insert_Closing_Bracket_Function_Body_264.h"

// ========================================================================================================
#define MUT264_OUTPUT 1

void MutatorFrontendAction_264::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("funcDecl")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (!FD->doesThisDeclarationHaveABody())
            return;

        auto body = FD->getBody();
        if (!body)
            return;

        SourceLocation openBraceLoc = body->getBeginLoc();
        if (openBraceLoc.isInvalid())
            return;

        SourceManager &SM = Rewrite.getSourceManager();
        SourceLocation insertLoc = SM.getLocForEndOfToken(openBraceLoc);
        if (insertLoc.isInvalid())
            return;

        Rewrite.InsertText(insertLoc, "]", true, true);
    }
}
  
void MutatorFrontendAction_264::MutatorASTConsumer_264::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(hasBody(compoundStmt())).bind("funcDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}