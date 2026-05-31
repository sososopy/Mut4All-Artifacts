//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_union_member_initialization_23
 */ 

class MutatorFrontendAction_23 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(23)
private:
    class MutatorASTConsumer_23 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_23(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_union_member_initialization_23.h"

// ========================================================================================================
#define MUT23_OUTPUT 1

void MutatorFrontendAction_23::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("UnionMember")) {
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       VD->getLocation()))
            return;

        if (VD->hasInit()) {
            auto initRange = VD->getInit()->getSourceRange();
            std::string replacement = "/*mut23*/";
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(initRange), replacement);
        }
    }
}
  
void MutatorFrontendAction_23::MutatorASTConsumer_23::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = varDecl(
        hasParent(recordDecl(isUnion())),
        hasInitializer(anything()),
        hasType(isInteger())
    ).bind("UnionMember");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}