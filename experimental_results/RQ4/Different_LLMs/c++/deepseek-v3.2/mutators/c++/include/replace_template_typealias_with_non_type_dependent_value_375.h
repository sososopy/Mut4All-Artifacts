//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_Typealias_With_Non_Type_Dependent_Value_375
 */ 
class MutatorFrontendAction_375 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(375)

private:
    class MutatorASTConsumer_375 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_375(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::ClassTemplateDecl*> templateDecls;
        std::vector<std::string> templateNames;
    };
};

