//header file
#pragma once
#include "Mutator_base.h"
#include "clang/AST/Decl.h"
#include "clang/AST/ExprCXX.h"
#include "clang/AST/Stmt.h"

/**
 * Replace_Lambda_ReturnType_With_Decltype_LocalVar_440
 */ 
class MutatorFrontendAction_440 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(440)

private:
    class MutatorASTConsumer_440 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_440(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Lambda_ReturnType_With_Decltype_LocalVar_440.h"
#include "clang/AST/ASTMatchers.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/Decl.h"
#include "clang/AST/ExprCXX.h"
#include "clang/AST/Stmt.h"
#include "clang/Rewrite/Core/Rewriter.h"

// ========================================================================================================
#define MUT440_OUTPUT 1

void MutatorFrontendAction_440::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *lambda = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambda")) {
        // Filter nodes in header files
        if (!lambda || !Result.Context->getSourceManager().isWrittenInMainFile(
                       lambda->getBeginLoc()))
            return;

        // Get enclosing function
        auto *function = dyn_cast<clang::FunctionDecl>(lambda->getDeclContext());
        if (!function)
            return;

        // Collect local variables in the function
        std::vector<const clang::VarDecl*> localVars;
        for (const auto *stmt : function->getBody()->children()) {
            if (const auto *varDecl = dyn_cast<clang::VarDecl>(stmt)) {
                if (!varDecl->isParameter())
                    localVars.push_back(varDecl);
            }
        }

        if (localVars.empty())
            return;

        // Select first local variable
        const clang::VarDecl *var = localVars[0];
        std::string varName = var->getNameAsString();

        // Get trailing return type source range
        clang::SourceRange returnTypeRange = lambda->getTrailingReturnTypeSourceRange();
        if (returnTypeRange.isInvalid())
            return;

        // Replace with decltype(var)
        std::string newReturnType = "decltype(" + varName + ")";
        Rewrite.ReplaceText(returnTypeRange, newReturnType);
    }
}
  
void MutatorFrontendAction_440::MutatorASTConsumer_440::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Match lambdas with explicit trailing return type in functions with local variables
    DeclarationMatcher matcher = lambdaExpr(
        hasTrailingReturnType(),
        unless(isImplicit())
    ).bind("lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}