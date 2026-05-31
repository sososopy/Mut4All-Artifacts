//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/DeclCXX.h"
#include "clang/AST/Decl.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Lex/Lexer.h"
#include <string>

/**
 * modify_friend_operator_context_550
 */ 
class MutatorFrontendAction_550 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(550)

private:
    class MutatorASTConsumer_550 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_550(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_friend_operator_context_550.h"

// ========================================================================================================
#define MUT550_OUTPUT 1

void MutatorFrontendAction_550::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("friendOp")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (FD->isOverloadedOperator() && FD->getFriendObjectKind() == clang::Decl::FOK_Friend) {
            auto sourceRange = FD->getSourceRange();
            std::string declText = clang::Lexer::getSourceText(clang::CharSourceRange::getTokenRange(sourceRange), *Result.SourceManager, clang::LangOptions()).str();

            // Remove 'friend' keyword
            size_t friendPos = declText.find("friend ");
            if (friendPos != std::string::npos) {
                declText.erase(friendPos, 7);
            }

            // Move the declaration outside the class
            std::string newDecl = "\n/*mut550*/" + declText + ";";
            Rewrite.ReplaceText(sourceRange, "");
            Rewrite.InsertTextAfterToken(FD->getEndLoc(), newDecl);
        }
    }
}

void MutatorFrontendAction_550::MutatorASTConsumer_550::HandleTranslationUnit(ASTContext &Context) {
    using namespace clang::ast_matchers;
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isFriend(), isOverloadedOperator()).bind("friendOp");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}