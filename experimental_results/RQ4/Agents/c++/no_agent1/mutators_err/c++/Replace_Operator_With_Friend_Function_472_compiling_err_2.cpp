//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/Tooling.h"

using namespace clang;
using namespace clang::ast_matchers;

/**
 * Replace_Operator_With_Friend_Function_472
 */ 

class MutatorFrontendAction_472 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(472)
private:
    class MutatorASTConsumer_472 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_472(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
    };
};

//source file
#include "../include/replace_operator_with_friend_function_472.h"

// ========================================================================================================
#define MUT472_OUTPUT 1

void MutatorFrontendAction_472::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *OP = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("OperatorMethod")) {
        if (!OP || !Result.Context->getSourceManager().isWrittenInMainFile(OP->getLocation()))
            return;

        if (OP->isOverloadedOperator() && OP->isUserProvided()) {
            auto operatorName = OP->getNameAsString();
            auto className = OP->getParent()->getNameAsString();
            auto returnType = OP->getReturnType().getAsString();
            std::string params;
            for (auto param : OP->parameters()) {
                if (!params.empty()) params += ", ";
                params += param->getType().getAsString() + " " + param->getNameAsString();
            }
            std::string friendFunction = "/*mut472*/friend " + returnType + " " + operatorName + "(" + className + " &lhs, " + params + ") {\n";
            friendFunction += "    // Implement friend function logic here\n";
            friendFunction += "}\n";

            Rewrite.ReplaceText(CharSourceRange::getTokenRange(OP->getSourceRange()), friendFunction);
        }
    }
}
  
void MutatorFrontendAction_472::MutatorASTConsumer_472::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxMethodDecl(isOverloadedOperator()).bind("OperatorMethod");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}