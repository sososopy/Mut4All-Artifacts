//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Unreachable_Default_Case_76
 */ 
class MutatorFrontendAction_76 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(76)

private:
    class MutatorASTConsumer_76 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_76(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/insert_unreachable_default_case_76.h"

// ========================================================================================================
#define MUT76_OUTPUT 1

void MutatorFrontendAction_76::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *SwitchStmt = Result.Nodes.getNodeAs<clang::SwitchStmt>("SwitchStmt")) {
        //Filter nodes in header files
        if (!SwitchStmt || !Result.Context->getSourceManager().isWrittenInMainFile(
                SwitchStmt->getBeginLoc()))
            return;

        //Get the source code text of target node
        auto switchBody = SwitchStmt->getBody();
        if (!switchBody)
            return;

        auto switchBodyText = stringutils::rangetoStr(*(Result.SourceManager), switchBody->getSourceRange());

        // Check if default case already exists
        if (switchBodyText.find("default:") != std::string::npos)
            return;

        //Perform mutation on the source code text by applying string replacement
        std::string unreachableDefault = "\n/*mut76*/default: __builtin_unreachable();\n";
        switchBodyText.insert(switchBodyText.rfind('}'), unreachableDefault);

        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(switchBody->getSourceRange()), switchBodyText);
    }
}

void MutatorFrontendAction_76::MutatorASTConsumer_76::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    StatementMatcher matcher = switchStmt().bind("SwitchStmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}