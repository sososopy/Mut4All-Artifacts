//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/AST/ASTContext.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Tooling/Tooling.h"
#include <memory>
#include <string>

/**
 * incorrect_template_specialization_7
 */ 
class MutatorFrontendAction_7 : public clang::ASTFrontendAction {
public:
    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &CI, llvm::StringRef) override {
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        return std::make_unique<MutatorASTConsumer_7>(TheRewriter);
    }

private:
    class MutatorASTConsumer_7 : public clang::ASTConsumer {
    public:
        MutatorASTConsumer_7(clang::Rewriter &R) : TheRewriter(R) {}
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
#include "../include/incorrect_template_specialization_7.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/AST/DeclCXX.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/AST/ASTContext.h"
#include <string>

using namespace clang;
using namespace clang::ast_matchers;

void MutatorFrontendAction_7::Callback::run(const MatchFinder::MatchResult &Result) {
    if (const auto *MT = Result.Nodes.getNodeAs<CXXMethodDecl>("TemplateMethods")) {
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(MT->getLocation()))
            return;

        if (MT->isTemplated() && MT->getTemplatedKind() == CXXMethodDecl::TK_FunctionTemplateSpecialization) {
            std::string declaration = TheRewriter.getRewrittenText(MT->getSourceRange());
            std::string incorrectSpecialization = "\n/*mut7*/template <> void " + MT->getNameAsString() + "<int>(int val) { /* Incorrect or incomplete logic */ }\n";
            declaration.insert(declaration.rfind("}"), incorrectSpecialization);
            Rewrite.ReplaceText(MT->getSourceRange(), declaration);
        }
    }
}

void MutatorFrontendAction_7::MutatorASTConsumer_7::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl(hasDescendant(cxxMethodDecl())).bind("TemplateMethods");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}