//header file
#pragma once
#include "Mutator_base.h"

/**
 * Substitute_Template_Argument_391
 */ 
class MutatorFrontendAction_391 : public MutatorFrontendAction {
public:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef InFile) override {
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        return std::make_unique<MutatorASTConsumer_391>(TheRewriter);
    }

private:
    class MutatorASTConsumer_391 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_391(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
        std::vector<const clang::TemplateDecl *> templateDecls;
    };
};

//source file
#include "../include/Substitute_Template_Argument_391.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "llvm/Support/SourceMgr.h"

// ========================================================================================================
#define MUT391_OUTPUT 1

void MutatorFrontendAction_391::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TD = Result.Nodes.getNodeAs<clang::TemplateDecl>("TemplateDecl")) {
      //Filter nodes in header files
      if (!TD ||!Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;
      //Record the template declaration
      templateDecls.push_back(TD);
    }
    else if (auto *TI = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("TemplateInstantiation")) {
      //Filter nodes in header files
      if (!TI ||!Result.Context->getSourceManager().isWrittenInMainFile(
                     TI->getLocation()))
        return;
      //Get the source code text of target node
      auto instantiation = clang::Lexer::getSourceText(
          clang::CharSourceRange::getTokenRange(TI->getSourceRange()), 
          *Result.SourceManager, 
          Result.Context->getLangOpts());
      //Perform mutation on the source code text by applying string replacement
      for (const auto &TD : templateDecls) {
        if (TD->getNameAsString() == TI->getTemplateName().getAsTemplateDecl()->getNameAsString()) {
          // Substitute template argument
          std::string newArgument = "NewType";
          instantiation.replace(instantiation.find("<"), instantiation.find(">"), "<" + newArgument + ">");
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(clang::CharSourceRange::getTokenRange(TI->getSourceRange()), instantiation);
        }
      }
    }
}
  
void MutatorFrontendAction_391::MutatorASTConsumer_391::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = templateDecl().bind("TemplateDecl");
    DeclarationMatcher instantiationMatcher = classTemplateSpecializationDecl().bind("TemplateInstantiation");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(instantiationMatcher, &callback);
    matchFinder.matchAST(Context);
}