//header file
#pragma once
#include "Mutator_base.h"

/**
 * Lambda_With_Function_Type_Parameter_320
 */ 
class MutatorFrontendAction_320 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(320)

private:
    class MutatorASTConsumer_320 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_320(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Lambda_With_Function_Type_Parameter_320.h"

// ========================================================================================================
#define MUT320_OUTPUT 1

void MutatorFrontendAction_320::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *lambda = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambda")) {
        if (!lambda || !Result.Context->getSourceManager().isWrittenInMainFile(
                       lambda->getBeginLoc()))
            return;

        if (auto *FD = dyn_cast<clang::FunctionDecl>(lambda->getDeclContext())) {
            auto lambdaRange = lambda->getSourceRange();
            std::string lambdaStr = stringutils::rangetoStr(*Result.SourceManager, lambdaRange);

            size_t openParen = lambdaStr.find('(');
            size_t closeParen = lambdaStr.find(')', openParen);
            if (openParen != std::string::npos && closeParen != std::string::npos) {
                std::string newParams = "void (func)()";
                std::string newLambdaStr = lambdaStr.substr(0, openParen + 1) + newParams + lambdaStr.substr(closeParen);
                Rewrite.ReplaceText(lambdaRange, newLambdaStr);
            }
        }
    }
}
  
void MutatorFrontendAction_320::MutatorASTConsumer_320::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = lambdaExpr(hasCaptures(), unless(isInitCapture())).bind("lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}