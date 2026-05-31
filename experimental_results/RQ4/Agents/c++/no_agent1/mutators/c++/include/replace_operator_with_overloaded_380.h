//header file
#pragma once
#include "Mutator_base.h"
#include <map>
#include <string>
#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/Rewrite/Core/Rewriter.h>

/**
 * Replace_Operator_With_Overloaded_380
 */ 
class MutatorFrontendAction_380 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(380)

private:
    class MutatorASTConsumer_380 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_380(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        std::map<const clang::CXXRecordDecl*, std::string> overloadedOperators;
    };
};

