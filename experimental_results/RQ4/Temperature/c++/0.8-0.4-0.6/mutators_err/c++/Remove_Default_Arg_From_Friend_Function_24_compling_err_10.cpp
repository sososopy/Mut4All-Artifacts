//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"

/**
 * remove_default_arg_from_friend_function_24
 */ 
class MutatorFrontendAction_24 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(24)

private:
    class MutatorASTConsumer_24 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_24(Rewriter &R) : TheRewriter(R) {}
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
        const clang::FunctionDecl *FriendFunctionDecl = nullptr;
    };
};

//source file
#include "../include/remove_default_arg_from_friend_function_24.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Rewrite/Core/Rewriter.h"

// ========================================================================================================
#define MUT24_OUTPUT 1

void MutatorFrontendAction_24::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FriendFunction = Result.Nodes.getNodeAs<clang::FunctionDecl>("FriendFunction")) {
        //Filter nodes in header files
        if (!FriendFunction || !Result.Context->getSourceManager().isWrittenInMainFile(
                FriendFunction->getLocation()))
            return;

        // Check if the function is a friend function with a default argument
        if (FriendFunction->getFriendObjectKind() == clang::Decl::FOK_None &&
            FriendFunction->getNumParams() > 0) {
            auto *LastParam = FriendFunction->getParamDecl(FriendFunction->getNumParams() - 1);
            if (LastParam->hasDefaultArg()) {
                // Record the function declaration for later use
                FriendFunctionDecl = FriendFunction;

                // Get the source code text of target node
                auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                           FriendFunction->getSourceRange());

                // Perform mutation on the source code text by removing the default argument
                std::string::size_type pos = declaration.rfind("=");
                if (pos != std::string::npos) {
                    declaration.erase(pos);
                }

                // Replace the original AST node with the mutated one
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(FriendFunction->getSourceRange()), declaration);
            }
        }
    } else if (auto *CallExpr = Result.Nodes.getNodeAs<clang::CallExpr>("FunctionCall")) {
        if (!CallExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                CallExpr->getBeginLoc()))
            return;

        // Check if the call expression corresponds to the friend function
        if (FriendFunctionDecl && CallExpr->getDirectCallee() == FriendFunctionDecl) {
            // Perform mutation on the source code text by removing the argument corresponding to the default argument
            if (CallExpr->getNumArgs() == FriendFunctionDecl->getNumParams()) {
                auto ArgRange = CallExpr->getArg(FriendFunctionDecl->getNumParams() - 1)->getSourceRange();
                Rewrite.RemoveText(CharSourceRange::getTokenRange(ArgRange));
            }
        }
    }
}

void MutatorFrontendAction_24::MutatorASTConsumer_24::HandleTranslationUnit(ASTContext &Context) {
    using namespace clang::ast_matchers;
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher friendFunctionMatcher = functionDecl(hasAttr<clang::FriendAttr>()).bind("FriendFunction");
    StatementMatcher functionCallMatcher = callExpr().bind("FunctionCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(friendFunctionMatcher, &callback);
    matchFinder.addMatcher(functionCallMatcher, &callback);
    matchFinder.matchAST(Context);
}