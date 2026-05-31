//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Friend_Operator_With_Vararg_236
 */ 
class MutatorFrontendAction_236 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(236)

private:
    class MutatorASTConsumer_236 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_236(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Friend_Operator_With_Vararg_236.h"

// ========================================================================================================
#define MUT236_OUTPUT 1

void MutatorFrontendAction_236::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *funcDecl = Result.Nodes.getNodeAs<clang::FunctionDecl>("func")) {
        if (!funcDecl->isFriend() || !funcDecl->isOperator())
            return;
        if (!Result.Context->getSourceManager().isWrittenInMainFile(funcDecl->getLocation()))
            return;
        SourceManager &SM = Result.Context->getSourceManager();
        SourceLocation startLoc = funcDecl->getBeginLoc();
        SourceLocation endLoc = funcDecl->getEndLoc();
        if (SM.isInvalidLoc(startLoc) || SM.isInvalidLoc(endLoc))
            return;
        std::string funcText = stringutils::rangetoStr(SM, CharSourceRange::getCharRange(startLoc, endLoc));
        size_t open = funcText.find('(');
        size_t close = funcText.find(')');
        if (open == std::string::npos || close == std::string::npos)
            return;
        std::string params = funcText.substr(open + 1, close - open - 1);
        std::vector<std::string> paramList;
        split(params, ",", paramList);
        if (paramList.empty())
            return;
        std::string newParams = paramList[0] + ", ...";
        funcText.replace(open + 1, close - open - 1, newParams);
        Rewrite.ReplaceText(CharSourceRange::getCharRange(startLoc, endLoc), funcText);
    }
}

void MutatorFrontendAction_236::MutatorASTConsumer_236::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isFriend(), isOperatorFunction()).bind("func");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}