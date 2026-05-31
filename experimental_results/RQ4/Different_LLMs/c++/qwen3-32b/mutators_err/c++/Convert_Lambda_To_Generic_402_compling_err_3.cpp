//header file
#pragma once
#include "Mutator_base.h"

/**
 * Convert_Lambda_To_Generic_402
 */ 
class MutatorFrontendAction_402 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(402)

private:
    class MutatorASTConsumer_402 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_402(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Convert_Lambda_To_Generic_402.h"

// ========================================================================================================
#define MUT402_OUTPUT 1

void MutatorFrontendAction_402::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambda")) {
        if (!LE || !Result.Context->getSourceManager().isWrittenInMainFile(LE->getBeginLoc()))
            return;
        
        auto SM = &Result.Context->getSourceManager();
        auto Loc = LE->getBeginLoc();
        if (SM->isInSystemHeader(Loc))
            return;
        
        auto SR = LE->getSourceRange();
        std::string lambdaCode = stringutils::rangetoStr(*SM, SR);
        
        size_t captureEnd = lambdaCode.find(']');
        if (captureEnd == std::string::npos)
            return;
        
        size_t bodyStart = lambdaCode.find('{', captureEnd);
        if (bodyStart == std::string::npos)
            return;
        
        std::string newParams = " (auto a, auto b)";
        std::string mutated = lambdaCode.substr(0, captureEnd + 1) + newParams + lambdaCode.substr(bodyStart);
        
        Rewrite.ReplaceText(SR, mutated);
    }
}

void MutatorFrontendAction_402::MutatorASTConsumer_402::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = lambdaExpr(
        hasAncestor(functionDecl()),
        hasCaptures(),
        has(cxxMethodDecl(
            unless(hasAnyParameter(decl(qualType(isAuto()))))
        ))
    ).bind("lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}