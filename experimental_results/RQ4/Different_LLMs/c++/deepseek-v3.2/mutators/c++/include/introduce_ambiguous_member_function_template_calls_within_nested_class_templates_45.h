//header file
#pragma once
#include "Mutator_base.h"

/**
 * Introduce_Ambiguous_Member_Function_Template_Calls_Within_Nested_Class_Templates_45
 */ 
class MutatorFrontendAction_45 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(45)

private:
    class MutatorASTConsumer_45 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_45(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl *> outerClasses;
    };
};

