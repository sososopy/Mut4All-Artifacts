//header file
#pragma once
#include "Mutator_base.h"

/**
 * Nested_Lambda_With_Pack_Expansion_In_Requires_Clause_117
 */ 
class MutatorFrontendAction_117 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(117)

private:
    class MutatorASTConsumer_117 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_117(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Nested_Lambda_With_Pack_Expansion_In_Requires_Clause_117.h"

// ========================================================================================================
#define MUT117_OUTPUT 1

void MutatorFrontendAction_117::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("LambdaExpr")) {
        if (!LE || !Result.Context->getSourceManager().isWrittenInMainFile(LE->getBeginLoc()))
            return;

        const auto *Body = LE->getBody();
        if (!Body)
            return;

        std::string lambdaText = stringutils::rangetoStr(*(Result.SourceManager), LE->getSourceRange());
        std::string mutatedLambda = lambdaText;

        size_t pos = mutatedLambda.find("return");
        if (pos != std::string::npos) {
            std::string nestedLambda = "[&](auto obj, auto... params) requires requires { obj.size(params...); } { return false; }";
            mutatedLambda.insert(pos, nestedLambda + "(obj); ");
        }

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(LE->getSourceRange()), mutatedLambda);
    }
}

void MutatorFrontendAction_117::MutatorASTConsumer_117::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = lambdaExpr(hasBody(compoundStmt(hasDescendant(lambdaExpr().bind("LambdaExpr"))))).bind("LambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}