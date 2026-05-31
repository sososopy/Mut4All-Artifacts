//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang;
using namespace clang::ast_matchers;

/**
 * modify_pseudo_destructor_usage_16
 */ 
class MutatorFrontendAction_16 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(16)

private:
    class MutatorASTConsumer_16 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_16(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/modify_pseudo_destructor_usage_16.h"

// ========================================================================================================
#define MUT16_OUTPUT 1

void MutatorFrontendAction_16::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *E = Result.Nodes.getNodeAs<clang::CXXPseudoDestructorExpr>("PseudoDtorExpr")) {
        if (!E || !Result.Context->getSourceManager().isWrittenInMainFile(E->getBeginLoc()))
            return;

        if (E->getDestroyedType().isNull())
            return;

        auto originalTypeLoc = E->getDestroyedTypeInfo()->getTypeLoc();
        std::string mutatedType = "UndefType";

        SourceRange typeRange = originalTypeLoc.getSourceRange();
        Rewrite.ReplaceText(typeRange, mutatedType);
    }
}
  
void MutatorFrontendAction_16::MutatorASTConsumer_16::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxPseudoDestructorExpr().bind("PseudoDtorExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}