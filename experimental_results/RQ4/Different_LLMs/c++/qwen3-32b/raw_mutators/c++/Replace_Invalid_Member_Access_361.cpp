//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Invalid_Member_Access_361
 */ 

class MutatorFrontendAction_361 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(361)

private:
    class MutatorASTConsumer_361 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_361(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Invalid_Member_Access_361.h"

// ========================================================================================================
#define MUT361_OUTPUT 1

void MutatorFrontendAction_361::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ME = Result.Nodes.getNodeAs<clang::MemberExpr>("memberAccess")) {
        if (!ME || !Result.Context->getSourceManager().isWrittenInMainFile(ME->getBeginLoc()))
            return;

        const auto *memberDecl = ME->getMemberDecl();
        if (!memberDecl)
            return;

        std::string originalName = memberDecl->getNameAsString();
        std::string newName = originalName + "_mut";

        SourceLocation nameLoc = ME->getMemberNameInfo().getLoc();
        if (nameLoc.isInvalid())
            return;

        unsigned nameLength = originalName.size();
        Rewrite.ReplaceText(nameLoc, nameLength, newName);
    }
}

void MutatorFrontendAction_361::MutatorASTConsumer_361::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = memberExpr().bind("memberAccess");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}