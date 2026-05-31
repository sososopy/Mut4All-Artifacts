//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Auto_Member_With_Default_Initializer_101
 */ 
class MutatorFrontendAction_101 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(101)

private:
    class MutatorASTConsumer_101 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_101(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl *> processedRecords;
    };
};

