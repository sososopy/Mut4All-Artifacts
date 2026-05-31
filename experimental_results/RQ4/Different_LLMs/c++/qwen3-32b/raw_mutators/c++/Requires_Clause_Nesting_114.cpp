//header file
#pragma once
#include "Mutator_base.h"

/**
 * Requires_Clause_Nesting_114
 */ 
class MutatorFrontendAction_114 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(114)

private:
    class MutatorASTConsumer_114 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_114(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Requires_Clause_Nesting_114.h"

// ========================================================================================================
#define MUT114_OUTPUT 1

void MutatorFrontendAction_114::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("func")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;
        if (!FD->getRequiresClause())
            return;
        auto params = FD->parameters();
        if (params.size() < 2)
            return;
        std::string param1 = params[0]->getNameAsString();
        std::string param2 = params[1]->getNameAsString();
        std::string inner_expr = param1 + " < " + param2;
        auto requiresClause = FD->getRequiresClause();
        SourceLocation startLoc = requiresClause->getBeginLoc();
        SourceLocation endLoc = requiresClause->getEndLoc();
        if (startLoc.isInvalid() || endLoc.isInvalid())
            return;
        auto SM = Result.Context->getSourceManager();
        std::string originalRequiresText = stringutils::rangetoStr(*SM, CharSourceRange::getTokenRange(startLoc, endLoc));
        size_t startBrace = originalRequiresText.find('{');
        size_t endBrace = originalRequiresText.rfind('}');
        if (startBrace == std::string::npos || endBrace == std::string::npos || startBrace >= endBrace)
            return;
        std::string body = originalRequiresText.substr(startBrace + 1, endBrace - startBrace - 1);
        body = body.substr(0, body.find_last_not_of(" \n\r\t") + 1);
        std::string newRequiresText = "requires (requires { " + inner_expr + "; } && requires { " + body + "; })";
        Rewrite.ReplaceText(startLoc, endLoc, newRequiresText);
    }
}

void MutatorFrontendAction_114::MutatorASTConsumer_114::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(hasRequiresClause()).bind("func");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}