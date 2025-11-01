//header file
#pragma once
#include "Mutator_base.h"

/**
 * Alter_Range_Based_For_Loop_100
 */ 
class MutatorFrontendAction_100 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(100)

private:
    class MutatorASTConsumer_100 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_100(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Alter_Range_Based_For_Loop_100.h"

// ========================================================================================================
#define MUT100_OUTPUT 1

void MutatorFrontendAction_100::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FL = Result.Nodes.getNodeAs<clang::CXXForRangeStmt>("ForLoop")) {
        if (!FL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FL->getForLoc()))
            return;

        auto &SM = Result.Context->getSourceManager();
        auto &LO = Result.Context->getLangOpts();
        auto LoopRange = FL->getSourceRange();
        auto LoopText = Lexer::getSourceText(CharSourceRange::getTokenRange(LoopRange), SM, LO);

        std::string condition = "true";
        std::string value1 = "1";
        std::string value2 = "2";

        std::string mutatedLoop = "bool condition = " + condition + ";\n";
        mutatedLoop += "int value1 = " + value1 + ";\n";
        mutatedLoop += "int value2 = " + value2 + ";\n";
        mutatedLoop += "for (auto x = condition ? value1 : value2 : ";

        size_t colonPos = LoopText.find(':');
        if (colonPos != std::string::npos) {
            mutatedLoop += LoopText.substr(colonPos + 1);
        }

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(LoopRange), mutatedLoop);
    }
}

void MutatorFrontendAction_100::MutatorASTConsumer_100::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    StatementMatcher matcher = cxxForRangeStmt().bind("ForLoop");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}