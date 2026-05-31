//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

/**
 * TemplateInstantiationWithIncompatibleType_235
 */ 
class MutatorFrontendAction_235 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(235)

private:
    class MutatorASTConsumer_235 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_235(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/TemplateInstantiationWithIncompatibleType_235.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTContext.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Rewrite/Core/Rewriter.h"

// ========================================================================================================
#define MUT235_OUTPUT 1

void MutatorFrontendAction_235::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateDecl>(("TemplateDecl"))) {
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
      std::string incompatibleType = "const char*";
      size_t pos = declaration.find('<');
      if (pos != std::string::npos) {
        size_t endPos = declaration.find('>');
        if (endPos != std::string::npos) {
          declaration.replace(pos + 1, endPos - pos - 1, incompatibleType);
        }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_235::MutatorASTConsumer_235::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = clang::ast_matchers::templateDecl().bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}