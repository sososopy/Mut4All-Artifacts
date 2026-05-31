//header file
#pragma once
#include "Mutator_base.h"

/**
 * Generic_Lambda_Default_Arg_23
 */ 
class MutatorFrontendAction_Generic_Lambda_Default_Arg_23 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(Generic_Lambda_Default_Arg_23)

private:
    class MutatorASTConsumer_Generic_Lambda_Default_Arg_23 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_Generic_Lambda_Default_Arg_23(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Generic_Lambda_Default_Arg_23.h"

// ========================================================================================================
#define MUTGeneric_Lambda_Default_Arg_23_OUTPUT 1

void MutatorFrontendAction_Generic_Lambda_Default_Arg_23::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *lambda = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambda")) {
        if (!lambda || !Result.Context->getSourceManager().isWrittenInMainFile(lambda->getBeginLoc()))
            return;

        if (!lambda->hasExplicitParameters())
            return;

        auto *callOp = lambda->getCallOperator();
        if (!callOp)
            return;

        const auto &params = callOp->parameters();
        if (params.size() < 1)
            return;

        const auto *firstParam = params[0];
        const auto paramRange = firstParam->getSourceRange();

        const CXXRecordDecl *closure = lambda->getLambdaClass();
        if (!closure)
            return;

        const FunctionDecl *func = dyn_cast<FunctionDecl>(closure->getDeclContext());
        const VarDecl *defaultVar = nullptr;
        if (func) {
            for (const auto *decl : func->decls()) {
                if (auto *vd = dyn_cast<VarDecl>(decl)) {
                    defaultVar = vd;
                    break;
                }
            }
        }

        std::string defaultArg;
        if (defaultVar) {
            defaultArg = defaultVar->getNameAsString();
        } else {
            defaultArg = "0";
        }

        std::string newParam = "auto " + firstParam->getNameAsString() + " = " + defaultArg;
        Rewrite.ReplaceText(paramRange, newParam);
    }
}

void MutatorFrontendAction_Generic_Lambda_Default_Arg_23::MutatorASTConsumer_Generic_Lambda_Default_Arg_23::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = clang::ast_matchers::lambdaExpr(clang::ast_matchers::has(clang::ast_matchers::cxxMethodDecl(clang::ast_matchers::hasNumParameters(1)))).bind("lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}