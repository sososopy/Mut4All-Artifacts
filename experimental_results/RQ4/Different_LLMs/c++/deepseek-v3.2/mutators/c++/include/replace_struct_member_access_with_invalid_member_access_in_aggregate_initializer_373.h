//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Struct_Member_Access_With_Invalid_Member_Access_In_Aggregate_Initializer_373
 */ 
class MutatorFrontendAction_373 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(373)

private:
    class MutatorASTConsumer_373 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_373(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl*> structDecls;
        std::vector<std::string> memberNames;
    };
};

