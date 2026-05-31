//header file
#pragma once
#include "Mutator_base.h"

/**
 * Remove_Comma_In_Requires_Clause_307
 */ 
class MutatorFrontendAction_307 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(307)

private:
    class MutatorASTConsumer_307 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_307(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Mutator_Remove_Comma_In_Requires_Clause_307.h"

// ========================================================================================================
#define MUT307_OUTPUT 1

void MutatorFrontendAction_307::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::RequiresExpr>("requiresExpr")) {
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(MT->getLocation()))
            return;

        auto constraints = MT->getConstraints();
        if (constraints.size() < 2)
            return;

        int index = getrandom::getRandomIndex(constraints.size() - 1);
        const auto *firstConstraint = constraints[index];
        const auto *secondConstraint = constraints[index + 1];

        SourceLocation firstEnd = firstConstraint->getEndLoc();
        SourceLocation secondStart = secondConstraint->getBeginLoc();

        // Get the text between firstEnd and secondStart
        std::string text;
        {
            SourceManager &SM = *Result.SourceManager;
            const char *start = SM.getCharacterData(firstEnd);
            const char *end = SM.getCharacterData(secondStart);
            text.assign(start, end - start);
        }

        size_t commaPos = text.find(',');
        if (commaPos == std::string::npos)
            return;

        SourceLocation commaLoc = firstEnd.getLocWithOffset(commaPos);
        Rewrite.RemoveText(commaLoc, 1);
    }
}
  
void MutatorFrontendAction_307::MutatorASTConsumer_307::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = requiresExpr().bind("requiresExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}