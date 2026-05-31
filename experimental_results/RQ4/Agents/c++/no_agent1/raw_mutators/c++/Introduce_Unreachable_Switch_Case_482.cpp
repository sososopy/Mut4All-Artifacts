//header file
#pragma once
#include "Mutator_base.h"

/**
 * Introduce_Unreachable_Switch_Case_482
 */ 
class MutatorFrontendAction_482 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(482)

private:
    class MutatorASTConsumer_482 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_482(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/introduce_unreachable_switch_case_482.h"

// ========================================================================================================
#define MUT482_OUTPUT 1

void MutatorFrontendAction_482::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *SW = Result.Nodes.getNodeAs<clang::SwitchStmt>("SwitchStmt")) {
        if (!SW || !Result.Context->getSourceManager().isWrittenInMainFile(
                       SW->getBeginLoc()))
            return;

        auto switchRange = SW->getSourceRange();
        auto switchText = stringutils::rangetoStr(*(Result.SourceManager), switchRange);
        
        // Insert an unreachable case in the switch statement
        std::string unreachableCase = "\n/*mut482*/ case 999999: break;";
        switchText.insert(switchText.rfind('}'), unreachableCase);
        
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(switchRange), switchText);
    }
}

void MutatorFrontendAction_482::MutatorASTConsumer_482::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    StatementMatcher matcher = switchStmt().bind("SwitchStmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}