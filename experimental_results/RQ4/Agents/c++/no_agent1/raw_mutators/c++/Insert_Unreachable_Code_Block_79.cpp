//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Unreachable_Code_Block_79
 */ 

class MutatorFrontendAction_79 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(79)
private:
    class MutatorASTConsumer_79 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_79(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/insert_unreachable_code_block_79.h"

// ========================================================================================================
#define MUT79_OUTPUT 1

void MutatorFrontendAction_79::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FS = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
        if (!FS || !Result.Context->getSourceManager().isWrittenInMainFile(
                   FS->getLocation()))
            return;

        if (FS->hasBody()) {
            auto body = FS->getBody();
            auto bodyText = stringutils::rangetoStr(*(Result.SourceManager), body->getSourceRange());
            
            std::string unreachableCode = "\n/*mut79*/if (false) { int unreachable = 0; unreachable++; }\n";
            bodyText.insert(bodyText.find("{") + 1, unreachableCode);
            
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(body->getSourceRange()), bodyText);
        }
    }
}

void MutatorFrontendAction_79::MutatorASTConsumer_79::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isDefinition()).bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}