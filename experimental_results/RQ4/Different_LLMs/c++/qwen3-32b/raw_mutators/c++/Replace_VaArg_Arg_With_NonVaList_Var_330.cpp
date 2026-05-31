//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_VaArg_Arg_With_NonVaList_Var_330
 */ 
class MutatorFrontendAction_330 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(330)

private:
    class MutatorASTConsumer_330 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_330(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_VaArg_Arg_With_NonVaList_Var_330.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/Decl.h"
#include "clang/AST/Expr.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "llvm/Support/raw_ostream.h"

// ========================================================================================================
#define MUT330_OUTPUT 1

using namespace clang;
using namespace clang::ast_matchers;

void MutatorFrontendAction_330::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *callExpr = Result.Nodes.getNodeAs<CallExpr>("vaArgCall")) {
        // Filter nodes in header files
        if (!callExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                           callExpr->getBeginLoc()))
            return;

        // Get the first argument
        Expr *arg0 = callExpr->getArg(0);
        QualType arg0Type = arg0->getType();
        if (!arg0Type->isVAListType()) {
            return; // First argument is not a va_list, skip
        }

        // Find variables in the same scope that are not va_list
        DeclContext *ctx = callExpr->getDeclContext();
        std::vector<VarDecl*> candidates;
        for (const auto *decl : ctx->decls()) {
            if (auto *varDecl = dyn_cast<VarDecl>(decl)) {
                QualType varType = varDecl->getType();
                if (!varType->isVAListType()) {
                    // Check if the variable is declared before the call
                    if (varDecl->getLocation().isBeforeInTranslationUnit(callExpr->getBeginLoc())) {
                        candidates.push_back(varDecl);
                    }
                }
            }
        }

        if (candidates.empty()) {
            return;
        }

        // Select the first candidate (for simplicity)
        VarDecl *selectedVar = candidates[0];
        std::string varName = selectedVar->getNameAsString();

        // Replace the first argument with the variable name
        SourceRange arg0Range = arg0->getSourceRange();
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(arg0Range), varName);
    }
}
  
void MutatorFrontendAction_330::MutatorASTConsumer_330::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define ASTMatcher to identify __builtin_va_arg calls
    DeclarationMatcher matcher = callExpr(
        callee(builtinFunctionDecl(hasName("__builtin_va_arg")))
    ).bind("vaArgCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}