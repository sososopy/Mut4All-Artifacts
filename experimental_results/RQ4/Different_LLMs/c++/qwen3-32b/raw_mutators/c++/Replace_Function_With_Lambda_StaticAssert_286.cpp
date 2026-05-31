//header file
#pragma once
#include "Mutator_base.h"
#include "clang/AST/Decl.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Rewrite/Core/Rewriter.h"

/**
 * Replace_Function_With_Lambda_StaticAssert_286
 */ 
class MutatorFrontendAction_286 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(286)

private:
    class MutatorASTConsumer_286 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_286(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Function_With_Lambda_StaticAssert_286.h"

// ========================================================================================================
#define MUT286_OUTPUT 1

void MutatorFrontendAction_286::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;
        if (!FD->hasBody())
            return;

        const Stmt *Body = FD->getBody();
        if (!Body)
            return;

        std::string newCode = R"(
            auto lambda = [x=42](auto y) {
                using F = decltype([x](auto z) { return x + z; });
                static_assert(std::is_same_v<decltype(std::declval<F>().operator()(std::declval<decltype(y)>()) * 0), int>);
                return x + y;
            };
            return lambda(x);
        )";

        newCode = newCode.substr(1, newCode.size() - 2);

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(Body->getSourceRange()), newCode);
    }
}

void MutatorFrontendAction_286::MutatorASTConsumer_286::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(unless(isConstexpr())).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}