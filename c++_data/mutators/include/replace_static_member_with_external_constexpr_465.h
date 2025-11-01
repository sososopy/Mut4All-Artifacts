//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Static_Member_With_External_Constexpr_465
 */ 
class MutatorFrontendAction_465 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(465)

private:
    class MutatorASTConsumer_465 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_465(Rewriter &R) : TheRewriter(R) {}
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
        std::map<std::string, std::string> staticMemberMap;
    };
};

