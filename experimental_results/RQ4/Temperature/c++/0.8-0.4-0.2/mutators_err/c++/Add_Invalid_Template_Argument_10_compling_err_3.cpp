//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Tooling/Tooling.h"
#include <memory>
#include <string>

/**
 * Add_Invalid_Template_Argument_10
 */ 
class MutatorFrontendAction_10 : public clang::ASTFrontendAction {
public:
    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &CI, llvm::StringRef) override {
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        return std::make_unique<MutatorASTConsumer_10>(TheRewriter);
    }

private:
    class MutatorASTConsumer_10 : public clang::ASTConsumer {
    public:
        MutatorASTConsumer_10(clang::Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(clang::ASTContext &Context) override;
    private:
        clang::Rewriter &TheRewriter;
    };
    
    class Callback : public clang::ast_matchers::MatchFinder::MatchCallback {
    public:
        Callback(clang::Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const clang::ast_matchers::MatchFinder::MatchResult &Result) override;
    private:
        clang::Rewriter &Rewrite;
    };

    clang::Rewriter TheRewriter;
};

//source file
#include "../include/add_invalid_template_argument_10.h"

// ========================================================================================================
#define MUT10_OUTPUT 1

void MutatorFrontendAction_10::Callback::run(const clang::ast_matchers::MatchFinder::MatchResult &Result) {
    if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplateCall")) {
        if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(FT->getLocation()))
            return;

        auto funcCall = clang::Lexer::getSourceText(clang::CharSourceRange::getTokenRange(FT->getSourceRange()), *Result.SourceManager, Result.Context->getLangOpts());

        size_t angleBracketPos = funcCall.find('<');
        if (angleBracketPos != std::string::npos) {
            funcCall.insert(angleBracketPos + 1, "int, 42, ");
        }

        Rewrite.ReplaceText(clang::CharSourceRange::getTokenRange(FT->getSourceRange()), funcCall);
    }
}

void MutatorFrontendAction_10::MutatorASTConsumer_10::HandleTranslationUnit(clang::ASTContext &Context) {
    clang::ast_matchers::MatchFinder matchFinder;
    clang::ast_matchers::DeclarationMatcher matcher = clang::ast_matchers::functionTemplateDecl().bind("FunctionTemplateCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}