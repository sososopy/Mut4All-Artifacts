//header file
#pragma once
#include "Mutator_base.h"

/**
 * Delete_Member_And_Friend_Functions_And_Ctor_262
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
        std::vector<const clang::CXXRecordDecl *> cur_classes;
        std::vector<const clang::CXXMethodDecl *> cur_methods;
        std::vector<const clang::CXXConstructorDecl *> cur_ctors;
        std::vector<const clang::FriendDecl *> cur_friends;
    };
};

