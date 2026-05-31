//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Unreachable_Code_329
 */ 
class MutatorFrontendAction_329 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(329)

private:
    class MutatorASTConsumer_329 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_329(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::FunctionDecl *> targetFunctions;
    };
};

//source file
#include "../include/insert_unreachable_code_329.h"

// ========================================================================================================
#define MUT329_OUTPUT 1

void MutatorFrontendAction_329::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FD->getLocation()))
            return;

        if (FD->hasBody()) {
            targetFunctions.push_back(FD);
            auto body = FD->getBody();
            auto bodyText = stringutils::rangetoStr(*(Result.SourceManager), body->getSourceRange());
            std::string unreachableCode = "\n/*mut329*/if (false) { __builtin_unreachable(); }\n";
            bodyText.insert(bodyText.find("{") + 1, unreachableCode);
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(body->getSourceRange()), bodyText);
        }
    }
}

void MutatorFrontendAction_329::MutatorASTConsumer_329::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isDefinition()).bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}