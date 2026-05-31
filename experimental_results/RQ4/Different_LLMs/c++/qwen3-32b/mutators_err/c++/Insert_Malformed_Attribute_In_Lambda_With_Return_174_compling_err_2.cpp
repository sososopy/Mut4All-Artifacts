//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Malformed_Attribute_In_Lambda_With_Return_174
 */ 
class MutatorFrontendAction_174 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(174)

private:
    class MutatorASTConsumer_174 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_174(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/Insert_Malformed_Attribute_In_Lambda_With_Return_174.h"

// ========================================================================================================
#define MUT174_OUTPUT 1

void MutatorFrontendAction_174::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambda")) {
        if (!LE || !Result.Context->getSourceManager().isWrittenInMainFile(LE->getBeginLoc()))
            return;

        CXXMethodDecl *callOp = LE->getCallOperator();
        if (!callOp || callOp->getNumParams() == 0)
            return;

        SourceManager &SM = *Result.SourceManager;
        const LangOptions &LangOpts = Result.Context->getLangOpts();
        SourceLocation startLoc = LE->getBeginLoc();
        SourceLocation endLoc = LE->getEndLoc();
        if (startLoc.isInvalid() || endLoc.isInvalid())
            return;

        CharSourceRange range = CharSourceRange::getTokenRange(SourceRange(startLoc, endLoc));
        std::string lambdaText = Lexer::getSourceText(range, SM, LangOpts).str();

        size_t paramEnd = lambdaText.find(')');
        if (paramEnd == std::string::npos)
            return;

        ParmVarDecl *firstParam = callOp->getParam(0);
        std::string paramName = firstParam->getNameAsString();
        if (paramName.empty()) {
            paramName = "0";
        }

        std::string attribute = "__attribute__((abc(({'return " + paramName + ";'})))";
        std::string mutatedLambda = lambdaText.substr(0, paramEnd + 1) + attribute + lambdaText.substr(paramEnd + 1);

        Rewrite.ReplaceText(range, mutatedLambda);
    }
}

void MutatorFrontendAction_174::MutatorASTConsumer_174::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    StatementMatcher matcher = lambdaExpr().bind("lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}