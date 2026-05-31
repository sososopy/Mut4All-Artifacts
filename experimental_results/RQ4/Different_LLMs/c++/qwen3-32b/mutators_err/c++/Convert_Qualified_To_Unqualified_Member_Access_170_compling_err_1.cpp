//header file
#pragma once
#include "Mutator_base.h"

/**
 * Convert_Qualified_To_Unqualified_Member_Access_170
 */ 
class MutatorFrontendAction_170 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(170)

private:
    class MutatorASTConsumer_170 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_170(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Convert_Qualified_To_Unqualified_Member_Access_170.h"

// ========================================================================================================
#define MUT170_OUTPUT 1

void MutatorFrontendAction_170::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *call = Result.Nodes.getNodeAs<clang::CXXMemberCallExpr>("call")) {
        if (!call || !Result.Context->getSourceManager().isWrittenInMainFile(
                           call->getBeginLoc()))
            return;

        const auto *memberExpr = dyn_cast<clang::CXXDependentScopeMemberExpr>(call->getCallee());
        if (!memberExpr)
            return;

        SourceLocation startLoc = memberExpr->getBeginLoc();
        SourceLocation endLoc = memberExpr->getEndLoc();
        std::string memberName = memberExpr->getMemberNameInfo().getName().getAsString();
        Rewrite.ReplaceText(startLoc, endLoc, memberName);
    }
}
  
void MutatorFrontendAction_170::MutatorASTConsumer_170::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxMemberCallExpr(
        hasImplicitObjectArgument(declRefExpr(to(thisExpr()))),
        callee(cxxDependentScopeMemberExpr())
    ).bind("call");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}