//header file
#pragma once
#include "Mutator_base.h"
#include <unordered_map>
#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/Rewrite/Core/Rewriter.h>
#include <clang/AST/ASTContext.h>
#include <clang/AST/Expr.h>
#include <clang/AST/Decl.h>
#include <clang/AST/Type.h>

using namespace clang;
using namespace clang::ast_matchers;

/**
 * modify_atomic_shift_operands_676
 */ 
class MutatorFrontendAction_676 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(676)

private:
    class MutatorASTConsumer_676 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_676(Rewriter &R) : TheRewriter(R) {}
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
        std::unordered_map<const clang::VarDecl*, std::string> atomicToRegularVarMap;
    };
};

