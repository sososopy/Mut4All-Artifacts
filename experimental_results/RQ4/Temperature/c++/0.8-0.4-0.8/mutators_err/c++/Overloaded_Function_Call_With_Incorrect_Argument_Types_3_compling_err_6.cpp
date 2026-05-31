//header file
#pragma once
#include "Mutator_base.h"

/**
 * overloaded_function_call_with_incorrect_argument_types_3
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
#include "../include/overloaded_function_call_with_incorrect_argument_types_3.h"

// ========================================================================================================
#define MUT3_OUTPUT 1

void MutatorFrontendAction_3::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FC = Result.Nodes.getNodeAs<clang::CallExpr>("FunctionCalls")) {
        if (!FC || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FC->getBeginLoc()))
            return;

        const FunctionDecl *FD = FC->getDirectCallee();
        if (!FD || FD->getOverloadedOperator() != OO_None)
            return;

        for (const auto *Overload : overloadedFunctions) {
            if (FD->getName() == Overload->getName() && FD != Overload) {
                std::vector<const Expr *> args;
                for (unsigned i = 0; i < FC->getNumArgs(); ++i) {
                    args.push_back(FC->getArg(i));
                }
                if (!args.empty()) {
                    std::string mutatedArg = "\"mutated_string\""; // Example of incorrect argument type
                    std::string callText = FD->getNameInfo().getName().getAsString() + "(" + mutatedArg + ");";
                    Rewrite.ReplaceText(CharSourceRange::getTokenRange(FC->getSourceRange()), callText);
                }
                break;
            }
        }
    } else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("OverloadedFunctions")) {
        overloadedFunctions.push_back(FD);
    }
}

void MutatorFrontendAction_3::MutatorASTConsumer_3::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto functionCallMatcher = callExpr(callee(functionDecl(hasOverloadedOperatorName("None")))).bind("FunctionCalls");
    auto overloadedFunctionMatcher = functionDecl(hasOverloadedOperatorName()).bind("OverloadedFunctions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(functionCallMatcher, &callback);
    matchFinder.addMatcher(overloadedFunctionMatcher, &callback);
    matchFinder.matchAST(Context);
}