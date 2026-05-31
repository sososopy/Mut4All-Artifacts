//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Variable_Template_Partial_Specialization_With_Class_Template_168
 */ 
class MutatorFrontendAction_168 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(168)

private:
    class MutatorASTConsumer_168 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_168(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result);
        std::map<std::string, std::string> varToWrapperMap;
        std::map<std::string, std::vector<clang::SourceRange>> usageRanges;
    private:
        Rewriter &Rewrite;
    };
};

