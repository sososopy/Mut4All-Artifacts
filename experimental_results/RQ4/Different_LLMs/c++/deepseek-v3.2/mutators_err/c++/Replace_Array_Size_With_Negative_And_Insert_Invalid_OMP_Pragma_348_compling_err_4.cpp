//header file
#ifndef MUTATOR_1_H
#define MUTATOR_1_H

#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Lex/Lexer.h"

class Mutator_1 : public clang::ASTConsumer,
                  public clang::RecursiveASTVisitor<Mutator_1> {
public:
    explicit Mutator_1(clang::ASTContext &C);
    void HandleTranslationUnit(clang::ASTContext &C) override;
    bool VisitVarDecl(clang::VarDecl *VD);
private:
    clang::Rewriter Rewrite;
};

class MutatorFrontendAction_1 : public clang::ASTFrontendAction {
public:
    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(
        clang::CompilerInstance &CI, clang::StringRef file) override {
        return std::make_unique<Mutator_1>(CI.getASTContext());
    }
};

#endif
//source file
#include "Mutator_1.h"

Mutator_1::Mutator_1(clang::ASTContext &C) : Rewrite(C.getSourceManager(), C.getLangOpts()) {}

void Mutator_1::HandleTranslationUnit(clang::ASTContext &C) {
    TraverseDecl(C.getTranslationUnitDecl());
    Rewrite.overwriteChangedFiles();
}

bool Mutator_1::VisitVarDecl(clang::VarDecl *VD) {
    if (VD->hasInit()) {
        clang::Expr *Init = VD->getInit();
        clang::SourceRange InitRange = Init->getSourceRange();
        clang::SourceManager &SM = VD->getASTContext().getSourceManager();
        std::string InitText = clang::Lexer::getSourceText(
            clang::CharSourceRange::getTokenRange(InitRange), SM,
            clang::LangOptions()).str();
        if (InitText.find("++") != std::string::npos ||
            InitText.find("--") != std::string::npos) {
            clang::SourceLocation Start = InitRange.getBegin();
            clang::SourceLocation End = InitRange.getEnd();
            std::string Replacement = "0";
            Rewrite.ReplaceText(clang::SourceRange(Start, End), Replacement);
        }
    }
    return true;
}