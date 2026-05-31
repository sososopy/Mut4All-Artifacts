//header file
#pragma once
#include "Mutator_base.h"
#include <map>
#include <string>
#include <clang/AST/ASTContext.h>
#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/Rewrite/Core/Rewriter.h>
#include <clang/Tooling/Tooling.h>

/**
 * modify_union_initialization_with_multiple_initializers_26
 */ 
class MutatorFrontendAction_26 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(26)

private:
    class MutatorASTConsumer_26 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_26(clang::Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(clang::ASTContext &Context) override;
    private:
        clang::Rewriter &TheRewriter;
    };
    
    class Callback : public clang::ast_matchers::MatchFinder::MatchCallback {
    public:
        Callback(clang::Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const clang::ast_matchers::MatchFinder::MatchResult &Result);
    private:
        clang::Rewriter &Rewrite;
        std::map<const clang::RecordDecl*, const clang::FieldDecl*> unionFields;
        std::string getDefaultValueForType(const clang::QualType &type);
    };
};

