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
    //Check whether the matched AST node is the target node
    if (const auto *PD = Result.Nodes.getNodeAs<clang::PreprocessingDirective>("EmbedDirective")) {
      //Filter nodes in header files
      if (!PD || !Result.SourceManager->isWrittenInMainFile(PD->getBeginLoc()))
        return;

      //Get the source code text of target node
      auto directiveText = clang::Lexer::getSourceText(clang::CharSourceRange::getTokenRange(PD->getSourceRange()), *Result.SourceManager, Result.Context->getLangOpts());
      
      //Perform mutation on the source code text by applying string replacement
      size_t pos = directiveText.find("__FILE__");
      if (pos != std::string::npos) {
        directiveText.insert(pos + 8, " unknownParam");
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(clang::CharSourceRange::getTokenRange(PD->getSourceRange()), directiveText);
      }
    }
}
  
void MutatorFrontendAction_32::MutatorASTConsumer_32::HandleTranslationUnit(clang::ASTContext &Context) {
    clang::ast_matchers::MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
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