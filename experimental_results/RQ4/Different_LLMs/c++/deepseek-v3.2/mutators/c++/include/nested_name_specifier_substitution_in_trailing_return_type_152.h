//header file
#pragma once
#include "Mutator_base.h"

/**
 * Nested_Name_Specifier_Substitution_In_Trailing_Return_Type_152
 */ 
class MutatorFrontendAction_152 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(152)

private:
    class MutatorASTConsumer_152 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_152(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<std::string> availableTypes;
        bool placeholderDeclared = false;
    };
};

