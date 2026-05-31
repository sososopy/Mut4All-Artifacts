//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Unreachable_Code_Block_459
 */ 
class MutatorFrontendAction_459 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(459)

private:
    class MutatorASTConsumer_459 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_459(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/add_unreachable_code_block_459.h"

// ========================================================================================================
#define MUT459_OUTPUT 1

void MutatorFrontendAction_459::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FD->getLocation()))
            return;

        if (FD->hasBody()) {
            auto body = FD->getBody();
            auto bodySourceRange = body->getSourceRange();
            auto bodyText = stringutils::rangetoStr(*(Result.SourceManager), bodySourceRange);

            std::string unreachableCode = "\n/*mut459*/if (false) {\n    int unreachable_var = 0;\n}\n";
            bodyText.insert(bodyText.find_last_of('}'), unreachableCode);

            Rewrite.ReplaceText(CharSourceRange::getTokenRange(bodySourceRange), bodyText);
        }
    }
}

void MutatorFrontendAction_459::MutatorASTConsumer_459::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isDefinition()).bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}