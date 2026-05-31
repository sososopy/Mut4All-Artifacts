//header file
#pragma once
#include "Mutator_base.h"

/**
 * Introduce_Constexpr_Incomplete_Struct_200
 */ 
class MutatorFrontendAction_200 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(200)

private:
    class MutatorASTConsumer_200 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_200(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result);
        void setNamesCollected(bool value) { namesCollected = value; }
        bool isNamesCollected() const { return namesCollected; }
        const std::set<std::string>& getExistingNames() const { return existingNames; }
        const std::vector<const clang::RecordDecl*>& getIncompleteStructs() const { return incompleteStructs; }
    private:
        Rewriter &Rewrite;
        std::vector<const clang::RecordDecl*> incompleteStructs;
        std::set<std::string> existingNames;
        bool namesCollected = false;
    };
};

