//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_consteval_function_invocation_638
 */ 
class MutatorFrontendAction_638 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(638)

private:
    class MutatorASTConsumer_638 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_638(Rewriter &R) : TheRewriter(R) {}
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
        std::map<const FunctionDecl*, std::string> constevalFunctions;
    };
};

//source file
#include "../include/modify_consteval_function_invocation_638.h"

// ========================================================================================================
#define MUT638_OUTPUT 1

void MutatorFrontendAction_638::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("ConstevalFunc")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (FD->isConsteval()) {
            // Get the source code text of target node
            auto declaration = stringutils::rangetoStr(*(Result.SourceManager), FD->getSourceRange());
            // Remove the consteval specifier
            size_t pos = declaration.find("consteval");
            if (pos != std::string::npos) {
                declaration.erase(pos, 9); // 9 is the length of "consteval"
                constevalFunctions[FD] = declaration;
                // Replace the original AST node with the mutated one
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), declaration);
            }
        }
    } else if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("ConstevalCall")) {
        if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(CE->getExprLoc()))
            return;

        const FunctionDecl *callee = CE->getDirectCallee();
        if (callee && constevalFunctions.find(callee) != constevalFunctions.end()) {
            // The function call matches a previously modified consteval function
            // No need to change the call itself as the function has been modified
        }
    }
}

void MutatorFrontendAction_638::MutatorASTConsumer_638::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define ASTMatchers to identify the target AST node for mutation
    auto constevalFuncMatcher = functionDecl(isConsteval()).bind("ConstevalFunc");
    auto constevalCallMatcher = callExpr(callee(functionDecl(isConsteval()))).bind("ConstevalCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(constevalFuncMatcher, &callback);
    matchFinder.addMatcher(constevalCallMatcher, &callback);
    matchFinder.matchAST(Context);
}