//header file
#pragma once
#include "Mutator_base.h"

/**
 * Introduce_Declval_In_Lambda_36
 */ 
class MutatorFrontendAction_36 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(36)

private:
    class MutatorASTConsumer_36 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_36(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/introduce_declval_in_lambda_36.h"

// ========================================================================================================
#define MUT36_OUTPUT 1

void MutatorFrontendAction_36::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("LambdaExpr")) {
        if (!LE || !Result.Context->getSourceManager().isWrittenInMainFile(
                    LE->getBeginLoc()))
            return;

        auto &SM = Rewrite.getSourceMgr();
        auto &LangOpts = Rewrite.getLangOpts();
        auto StartLoc = LE->getBeginLoc();
        auto EndLoc = LE->getEndLoc();

        std::string LambdaText = Lexer::getSourceText(CharSourceRange::getTokenRange(StartLoc, EndLoc), SM, LangOpts).str();
        
        std::string MutatedLambdaText = LambdaText;
        size_t BodyPos = MutatedLambdaText.find("{");
        if (BodyPos != std::string::npos) {
            std::string InsertText = "using Callable = decltype(*this);\n"
                                     "static_assert(std::is_same_v<decltype(std::declval<Callable>().operator()(std::declval<int>(), std::declval<double>())), double>, \"type mismatch\");\n";
            MutatedLambdaText.insert(BodyPos + 1, InsertText);
        }

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(StartLoc, EndLoc), MutatedLambdaText);
    }
}
  
void MutatorFrontendAction_36::MutatorASTConsumer_36::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = lambdaExpr(hasAnyParameter(anything())).bind("LambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}