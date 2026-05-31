//header file
#pragma once
#include "Mutator_base.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Tooling/Refactoring.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Basic/SourceLocation.h"
#include "llvm/Support/raw_ostream.h"

/**
 * embed_directive_mutation_32
 */ 
class MutatorFrontendAction_32 : public clang::ASTFrontendAction {
public:
    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &CI, llvm::StringRef InFile) override;

private:
    class MutatorASTConsumer_32 : public clang::ASTConsumer {
    public:
        MutatorASTConsumer_32(clang::Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(clang::ASTContext &Context) override;
    private:
        clang::Rewriter &TheRewriter;
    };
    
    class Callback : public clang::ast_matchers::MatchFinder::MatchCallback {
    public:
        Callback(clang::Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const clang::ast_matchers::MatchFinder::MatchResult &Result);
    private:
        clang::Rewriter &Rewrite;
    };
};

//source file
#include "../include/embed_directive_mutation_32.h"

// ========================================================================================================
#define MUT32_OUTPUT 1

void MutatorFrontendAction_32::Callback::run(const clang::ast_matchers::MatchFinder::MatchResult &Result) {
    if (const auto *PD = Result.Nodes.getNodeAs<clang::PreprocessingDirective>("EmbedDirective")) {
      if (!PD || !Result.SourceManager->isWrittenInMainFile(PD->getSourceRange().getBegin()))
        return;

      auto directiveText = clang::Lexer::getSourceText(clang::CharSourceRange::getTokenRange(PD->getSourceRange()), *Result.SourceManager, Result.Context->getLangOpts());
      
      std::string directiveStr = directiveText.str();
      size_t pos = directiveStr.find("__FILE__");
      if (pos != std::string::npos) {
        directiveStr.insert(pos + 8, " unknownParam");
        Rewrite.ReplaceText(clang::CharSourceRange::getTokenRange(PD->getSourceRange()), directiveStr);
      }
    }
}
  
void MutatorFrontendAction_32::MutatorASTConsumer_32::HandleTranslationUnit(clang::ASTContext &Context) {
    clang::ast_matchers::MatchFinder matchFinder;
    auto matcher = clang::ast_matchers::preprocessingDirective().bind("EmbedDirective");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
} 

std::unique_ptr<clang::ASTConsumer> MutatorFrontendAction_32::CreateASTConsumer(clang::CompilerInstance &CI, llvm::StringRef InFile) {
    clang::Rewriter TheRewriter;
    TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
    return std::make_unique<MutatorASTConsumer_32>(TheRewriter);
}