//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Unreachable_Code_468
 */ 
class MutatorFrontendAction_468 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(468)

private:
    class MutatorASTConsumer_468 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_468(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/insert_unreachable_code_468.h"

// ========================================================================================================
#define MUT468_OUTPUT 1

void MutatorFrontendAction_468::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FS = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
        if (!FS || !Result.Context->getSourceManager().isWrittenInMainFile(FS->getLocation()))
            return;

        if (FS->hasBody()) {
            auto body = FS->getBody();
            auto bodyText = stringutils::rangetoStr(*(Result.SourceManager), body->getSourceRange());

            std::string unreachableCode = "\n/*mut468*/if (false) { while(true) {} }\n";
            bodyText.insert(bodyText.find("{") + 1, unreachableCode);

            Rewrite.ReplaceText(CharSourceRange::getTokenRange(body->getSourceRange()), bodyText);
        }
    }
}

void MutatorFrontendAction_468::MutatorASTConsumer_468::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(hasBody()).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}