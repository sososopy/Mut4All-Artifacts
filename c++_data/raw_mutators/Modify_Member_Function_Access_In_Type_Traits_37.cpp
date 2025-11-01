//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Member_Function_Access_In_Type_Traits_37
 */ 
class MutatorFrontendAction_37 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(37)

private:
    class MutatorASTConsumer_37 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_37(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_member_function_access_in_type_traits_37.h"

// ========================================================================================================
#define MUT37_OUTPUT 1

void MutatorFrontendAction_37::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("LambdaExpr")) {
        if (!LE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LE->getBeginLoc()))
            return;

        auto lambdaSource = stringutils::rangetoStr(*(Result.SourceManager), LE->getSourceRange());

        size_t pos = lambdaSource.find("::Func()");
        if (pos != std::string::npos) {
            lambdaSource.replace(pos, 7, "::NonExistentFunc()");
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(LE->getSourceRange()), lambdaSource);
        }
    }
}

void MutatorFrontendAction_37::MutatorASTConsumer_37::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = lambdaExpr(hasDescendant(decltypeType())).bind("LambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}