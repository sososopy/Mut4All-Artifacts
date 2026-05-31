//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Aggregate_Initializer_218
 */ 
class MutatorFrontendAction_218 : public MutatorFrontendAction {
public:
    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &CI, llvm::StringRef InFile) override {
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        return std::make_unique<MutatorASTConsumer_218>(TheRewriter);
    }

private:
    class MutatorASTConsumer_218 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_218(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Modify_Aggregate_Initializer_218.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

// ========================================================================================================
#define MUT218_OUTPUT 1

void MutatorFrontendAction_218::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::VarDecl>("VarDecl")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = clang::Lexer::getSourceText(clang::CharSourceRange::getTokenRange(MT->getSourceRange()), *Result.SourceManager, Result.Context->getLangOpts());
      //Perform mutation on the source code text by applying string replacement
      if (MT->hasInit()) {
        auto init = MT->getInit();
        if (auto initList = clang::dyn_cast<clang::InitListExpr>(init)) {
          if (initList->getNumInits() > 0) {
            // Replace the initializer with an empty initializer list
            std::string newDeclaration = declaration.str();
            newDeclaration.replace(newDeclaration.find_first_of('{'), newDeclaration.find_last_of('}') - newDeclaration.find_first_of('{') + 1, "{}");
            declaration = newDeclaration;
          } else {
            // Remove the initializer altogether
            std::string newDeclaration = declaration.str();
            newDeclaration.erase(newDeclaration.find_first_of('='));
            declaration = newDeclaration;
          }
        }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(clang::CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_218::MutatorASTConsumer_218::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(hasInitializer(anyOf(hasType(isInteger()), hasType(floatType())))).bind("VarDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}