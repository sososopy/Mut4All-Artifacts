//header file
#pragma once
#include "Mutator_base.h"

/**
 * lambda_auto_initializer_674
 */ 
class MutatorFrontendAction_674 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(674)

private:
    class MutatorASTConsumer_674 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_674(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/lambda_auto_initializer_674.h"

// ========================================================================================================
#define MUT674_OUTPUT 1

void MutatorFrontendAction_674::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("autoVar")) {
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(VD->getLocation()))
            return;

        if (VD->hasInit() && VD->getType()->isDependentType()) {
            auto initExpr = VD->getInit();
            std::string initText = Lexer::getSourceText(CharSourceRange::getTokenRange(initExpr->getSourceRange()), 
                                                        *Result.SourceManager, 
                                                        Result.Context->getLangOpts()).str();
            std::string lambdaInit = "[]() { return " + initText + "; }()";

            Rewrite.ReplaceText(initExpr->getSourceRange(), lambdaInit);
        }
    }
}
  
void MutatorFrontendAction_674::MutatorASTConsumer_674::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = varDecl(hasType(autoType()), hasInitializer(expr())).bind("autoVar");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}