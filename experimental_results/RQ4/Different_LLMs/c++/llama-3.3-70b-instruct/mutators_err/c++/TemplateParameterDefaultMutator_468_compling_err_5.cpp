//header file
#pragma once
#include "Mutator_base.h"

/**
 * TemplateParameterDefaultMutator_468
 */ 
class MutatorFrontendAction_468 : public clang::ASTFrontendAction {
public:
    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &CI, llvm::StringRef InFile) override;

private:
    class MutatorASTConsumer_468 : public clang::ASTConsumer {
    public:
        MutatorASTConsumer_468(clang::Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(clang::ASTContext &Context) override;
    private:
        clang::Rewriter &TheRewriter;
    
    };
    
    class Callback : public clang::ast_matchers::MatchFinder::MatchCallback {
    public:
        Callback(clang::Rewriter &Rewrite, clang::ASTContext &Context, clang::SourceManager &SourceManager) 
            : Rewrite(Rewrite), Context(Context), SourceManager(SourceManager) {}
        virtual void run(const clang::ast_matchers::MatchFinder::MatchResult &Result) override;
    private:
        clang::Rewriter &Rewrite;
        clang::ASTContext &Context;
        clang::SourceManager &SourceManager;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Mutator_TemplateParameterDefaultMutator_468.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/SourceMgr.h"

// ========================================================================================================
#define MUT468_OUTPUT 1

std::unique_ptr<clang::ASTConsumer> MutatorFrontendAction_468::CreateASTConsumer(clang::CompilerInstance &CI, llvm::StringRef InFile) {
    clang::Rewriter TheRewriter;
    TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
    return std::make_unique<MutatorASTConsumer_468>(TheRewriter);
}

void MutatorFrontendAction_468::Callback::run(const clang::ast_matchers::MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateDecl")) {
      //Filter nodes in header files
      if (!MT || !SourceManager.isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = clang::Lexer::getSourceText(
          clang::CharSourceRange::getTokenRange(MT->getSourceRange()), 
          *SourceManager, 
          Context.getLangOpts());
      //Perform mutation on the source code text by applying string replacement
      // Find the default template argument
      if (MT->getTemplateParameters()->size() > 0) {
        auto templateParams = MT->getTemplateParameters();
        for (const auto &param : *templateParams) {
          if (const auto *templateParam = dyn_cast<clang::TemplateTypeParmDecl>(param)) {
            if (templateParam->hasDefaultArgument()) {
              // Replace the default template argument with a new one
              auto defaultArg = templateParam->getDefaultArgument()->getAsString(Context);
              auto newDefaultArg = "double"; // Replace with a new default argument
              declaration = declaration.substr(0, declaration.find(defaultArg)) + newDefaultArg + declaration.substr(declaration.find(defaultArg) + defaultArg.length());
            }
          }
        }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(clang::CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_468::MutatorASTConsumer_468::HandleTranslationUnit(clang::ASTContext &Context) {
    clang::ast_matchers::MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    clang::ast_matchers::DeclarationMatcher matcher = clang::ast_matchers::classTemplateDecl().bind("TemplateDecl");
    Callback callback(TheRewriter, Context, TheRewriter.getSourceMgr());
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}