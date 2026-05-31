//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_function_overload_resolution_with_ambiguous_types_3
 */ 
class MutatorFrontendAction_3 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(3)

private:
    class MutatorASTConsumer_3 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_3(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::FunctionDecl *> overloadedFunctions;
    };
};

//source file
#include "../include/modify_function_overload_resolution_with_ambiguous_types_3.h"

// ========================================================================================================
#define MUT3_OUTPUT 1

void MutatorFrontendAction_3::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (FD->isOverloadedOperator() || FD->isTemplateInstantiation())
            return;

        overloadedFunctions.push_back(FD);
    } else if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("CallExpr")) {
        if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(CE->getExprLoc()))
            return;

        for (auto *FD : overloadedFunctions) {
            if (FD->getNameInfo().getName().getAsString() == CE->getDirectCallee()->getNameInfo().getName().getAsString()) {
                auto args = CE->getArgs();
                for (unsigned i = 0; i < CE->getNumArgs(); ++i) {
                    if (args[i]->getType()->isIntegerType()) {
                        std::string replacement = std::to_string(42.0f);
                        Rewrite.ReplaceText(args[i]->getSourceRange(), replacement);
                        break;
                    }
                }
                break;
            }
        }
    }
}

void MutatorFrontendAction_3::MutatorASTConsumer_3::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher functionMatcher = functionDecl(anyOf(isOverloadedOperator(), isTemplateInstantiation())).bind("Functions");
    StatementMatcher callExprMatcher = callExpr(callee(functionDecl(anyOf(isOverloadedOperator(), isTemplateInstantiation())))).bind("CallExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(functionMatcher, &callback);
    matchFinder.addMatcher(callExprMatcher, &callback);
    matchFinder.matchAST(Context);
}