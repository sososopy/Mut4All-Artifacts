//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_VaArg_With_VLA_404
 */ 
class MutatorFrontendAction_404 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(404)

private:
    class MutatorASTConsumer_404 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_404(Rewriter &R) : TheRewriter(R) {}
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
        // Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Replace_VaArg_With_VLA_404.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/Decl.h"
#include "clang/AST/Expr.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Lex/Lexer.h"
#include "llvm/Support/raw_ostream.h"

// ========================================================================================================
#define MUT404_OUTPUT 1

void MutatorFrontendAction_404::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *call = Result.Nodes.getNodeAs<clang::CallExpr>("vaArgCall")) {
        // Filter nodes in header files
        if (!call || !Result.Context->getSourceManager().isWrittenInMainFile(
                           call->getBeginLoc()))
            return;

        // Check if the second argument is a pointer type
        auto *arg1 = call->getArg(1);
        QualType argType = arg1->getType();
        if (!argType->isPointerType()) {
            return;
        }

        // Get the function declaration
        auto *FD = dyn_cast<FunctionDecl>(call->getDeclContext());
        if (!FD) {
            return;
        }

        // Find an integer variable in the function's scope
        std::vector<const VarDecl*> intVars;
        for (const auto *decl : FD->decls()) {
            if (auto *var = dyn_cast<VarDecl>(decl)) {
                if (var->getType()->isIntegralOrEnumerationType()) {
                    intVars.push_back(var);
                }
            }
        }

        if (intVars.empty()) {
            return;
        }

        // Pick a random variable (for simplicity, pick the first one)
        const VarDecl *sizeVar = intVars[0];
        std::string sizeVarName = sizeVar->getNameAsString();

        // Get the first argument's name (va_list variable)
        auto *arg0 = call->getArg(0);
        std::string argsName = "";
        if (auto *declRef = dyn_cast<DeclRefExpr>(arg0)) {
            argsName = declRef->getNameInfo().getName().getAsString();
        } else {
            return;
        }

        // Get the pointee type of the original pointer type
        QualType pointeeType = argType->getPointeeType();
        std::string typeName = pointeeType.getAsString();

        // Construct the new type: T[sizeVarName]
        std::string newType = typeName + "[" + sizeVarName + "]";

        // Construct the new call: va_arg(args, T[sizeVarName])
        std::string newCall = "va_arg(" + argsName + ", " + newType + ")";

        // Construct the casted call: (T*)va_arg(...)
        std::string castedCall = "(" + typeName + "*)" + newCall;

        // Replace the original call with the mutated one
        SourceLocation start = call->getBeginLoc();
        SourceLocation end = call->getEndLoc();
        std::string originalText = clang::Lexer::getSourceText(
            clang::CharSourceRange::getTokenRange(start, end),
            *Result.SourceManager,
            Result.Context->getLangOpts()
        );
        Rewrite.ReplaceText(start, originalText.size(), castedCall);
    }
}

void MutatorFrontendAction_404::MutatorASTConsumer_404::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define ASTMatcher to find va_arg calls with pointer type as second argument
    DeclarationMatcher matcher = callExpr(
        callee(functionDecl(hasName("va_arg"))),
        hasArgument(0, expr()),
        hasArgument(1, expr())
    ).bind("vaArgCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}