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

/**
 * Modify_Template_Concept_Usage_128
 */ 
class MutatorFrontendAction_128 : public clang::ASTFrontendAction {
public:
    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &CI, llvm::StringRef) override {
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        return std::make_unique<MutatorASTConsumer_128>(TheRewriter);
    }

private:
    class MutatorASTConsumer_128 : public clang::ASTConsumer {
    public:
        MutatorASTConsumer_128(clang::Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Modify_Template_Concept_Usage_128.h"

#define MUT128_OUTPUT 1

void MutatorFrontendAction_128::Callback::run(const clang::ast_matchers::MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionWithConcept")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (FD->hasBody()) {
            auto functionBody = clang::Lexer::getSourceText(clang::CharSourceRange::getTokenRange(FD->getBody()->getSourceRange()), *Result.SourceManager, clang::LangOptions(), 0);
            std::string ifConstruct = "\nif constexpr (C<T>) {\n  // Operation depending on T\n} else {\n  // Ambiguous path\n}\n";
            std::string functionBodyStr = std::string(functionBody);
            functionBodyStr.insert(functionBodyStr.find("{") + 1, ifConstruct);
            Rewrite.ReplaceText(clang::CharSourceRange::getTokenRange(FD->getBody()->getSourceRange()), functionBodyStr);
        }
    }
}

void MutatorFrontendAction_128::MutatorASTConsumer_128::HandleTranslationUnit(clang::ASTContext &Context) {
    clang::ast_matchers::MatchFinder matchFinder;
    using namespace clang::ast_matchers;
    DeclarationMatcher matcher = functionDecl(hasDescendant(declRefExpr(to(conceptSpecializationExpr().bind("FunctionWithConcept")))));
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}