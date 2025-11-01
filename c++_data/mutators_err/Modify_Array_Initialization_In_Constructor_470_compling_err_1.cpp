//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Array_Initialization_In_Constructor_470
 */ 
class MutatorFrontendAction_470 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(470)

private:
    class MutatorASTConsumer_470 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_470(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Modify_Array_Initialization_In_Constructor_470.h"

// ========================================================================================================
#define MUT470_OUTPUT 1

void MutatorFrontendAction_470::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CtorInit = Result.Nodes.getNodeAs<clang::CXXCtorInitializer>("ctorInit")) {
        if (!CtorInit || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CtorInit->getSourceLocation()))
            return;

        if (auto *ArrayType = CtorInit->getType()->getAsArrayTypeUnsafe()) {
            std::string replacement = "{0, 1}";  // Example replacement with mismatched size
            Rewrite.ReplaceText(CtorInit->getSourceRange(), replacement);
        }
    }
}
  
void MutatorFrontendAction_470::MutatorASTConsumer_470::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxConstructorDecl(
        forEachConstructorInitializer(
            cxxCtorInitializer(
                isWrittenInMainFile(),
                withInitializer(expr(anyOf(
                    cxxNewExpr(hasType(arrayType())),
                    initListExpr(hasType(arrayType()))
                )))
            ).bind("ctorInit")
        )
    );
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}