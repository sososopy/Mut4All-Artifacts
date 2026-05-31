//header file
#pragma once
#include "Mutator_base.h"

/**
 * Introduce_Incomplete_Class_Declaration_Before_Member_Function_Body_122
 */ 
class MutatorFrontendAction_122 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(122)

private:
    class MutatorASTConsumer_122 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_122(Rewriter &R) : TheRewriter(R) {}
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
        std::set<std::string> existingTypeNames;
        std::string generateUniqueTypeName();
    };
};

