//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Unreachable_Statements_357
 */ 
class MutatorFrontendAction_357 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(357)

private:
    class MutatorASTConsumer_357 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_357(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/insert_unreachable_statements_357.h"

// ========================================================================================================
#define MUT357_OUTPUT 1

void MutatorFrontendAction_357::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FD->getLocation()))
            return;
        if (!FD->hasBody())
            return;

        auto body = FD->getBody();
        auto bodyStr = stringutils::rangetoStr(*(Result.SourceManager), body->getSourceRange());

        // Insert unreachable code
        std::string unreachableCode = "\n/*mut357*/ if (false) { volatile int x = 0; x++; }\n";
        bodyStr.insert(bodyStr.find("{") + 1, unreachableCode);

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(body->getSourceRange()), bodyStr);
    }
}

void MutatorFrontendAction_357::MutatorASTConsumer_357::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isDefinition()).bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}