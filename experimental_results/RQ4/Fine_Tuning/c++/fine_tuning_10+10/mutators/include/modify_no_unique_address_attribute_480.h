//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_No_Unique_Address_Attribute_480
 */ 
class MutatorFrontendAction_480 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(480)

private:
    class MutatorASTConsumer_480 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_480(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const FieldDecl *> noUniqueAddressFields;
        std::vector<const FieldDecl *> otherFields;
    };
};

