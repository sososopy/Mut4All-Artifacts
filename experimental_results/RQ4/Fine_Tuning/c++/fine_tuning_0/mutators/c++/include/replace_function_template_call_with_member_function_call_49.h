//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Function_Template_Call_With_Member_Function_Call_49
 */ 
class MutatorFrontendAction_49 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(49)

private:
    class MutatorASTConsumer_49 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_49(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl *> cur_classes;
        std::vector<const clang::CXXMethodDecl *> cur_methods;
    };
};

