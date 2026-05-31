//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"

/**
 * TemplateInstantiationWithDefaultArgumentMutation_439
 */ 
class MutatorFrontendAction_TemplateInstantiationWithDefaultArgumentMutation_439 : public MutatorFrontendAction {
public:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef InFile) override {
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        return std::make_unique<MutatorASTConsumer_TemplateInstantiationWithDefaultArgumentMutation_439>(TheRewriter);
    }

private:
    class MutatorASTConsumer_TemplateInstantiationWithDefaultArgumentMutation_439 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_TemplateInstantiationWithDefaultArgumentMutation_439(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Mutator_TemplateInstantiationWithDefaultArgumentMutation_439.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/AST/ASTContext.h"

// ========================================================================================================
#define MUT439_OUTPUT 1

void MutatorFrontendAction_TemplateInstantiationWithDefaultArgumentMutation_439::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateDecl>("TemplateDecl")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = clang::Lexer::getSourceText(
          CharSourceRange::getTokenRange(MT->getSourceRange()), 
          *Result.SourceManager, 
          Result.Context->getLangOpts());
      //Perform mutation on the source code text by applying string replacement
      // Replace default argument with a new expression
      auto defaultArgs = MT->getTemplateParameters();
      if (defaultArgs && defaultArgs->size() > 0) {
        auto newExpr = "sizeof(int)";
        std::string declStr = declaration.str();
        size_t pos = declStr.find(defaultArgs->begin()->getNameAsString());
        if (pos != std::string::npos) {
            declStr.replace(pos, defaultArgs->begin()->getNameAsString().size(), newExpr);
        }
        declaration = clang::StringRef(declStr);
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_TemplateInstantiationWithDefaultArgumentMutation_439::MutatorASTConsumer_TemplateInstantiationWithDefaultArgumentMutation_439::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = clang::ast_matchers::classTemplateDecl().bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}