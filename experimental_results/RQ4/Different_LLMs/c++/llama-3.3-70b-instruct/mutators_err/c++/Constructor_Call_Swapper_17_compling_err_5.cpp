//header file
#pragma once
#include "Mutator_base.h"

/**
 * Constructor_Call_Swapper_17
 */ 
class MutatorFrontendAction_17 : public MutatorFrontendAction {
public:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef File) override {
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        return std::make_unique<MutatorASTConsumer_17>(TheRewriter);
    }

private:
    class MutatorASTConsumer_17 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_17(Rewriter &R) : TheRewriter(R) {}
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
    Rewriter TheRewriter;
};

//source file
#include "../include/Mutator_Constructor_Call_Swapper_17.h"

// ========================================================================================================
#define MUT17_OUTPUT 1

void MutatorFrontendAction_17::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *C = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("Constructor")) {
      //Filter nodes in header files
      if (!C || !Result.Context->getSourceManager().isWrittenInMainFile(
                     C->getLocation()))
        return;
      //Get the source code text of target node
      auto initList = C->getInitList();
      if (initList && initList->getNumInits() > 1) {
        std::vector<clang::CXXCtorInitializer*> inits;
        for (auto init : *initList) {
          inits.push_back(init);
        }
        if (inits.size() > 1) {
          // Swap two random arguments
          int idx1 = getrandom::getRandomIndex(inits.size() - 1);
          int idx2 = getrandom::getRandomIndex(inits.size() - 1);
          while (idx2 == idx1) {
            idx2 = getrandom::getRandomIndex(inits.size() - 1);
          }
          std::swap(inits[idx1], inits[idx2]);
          // Replace the original AST node with the mutated one
          std::string mutatedCode = "";
          for (auto init : inits) {
            mutatedCode += init->getInit()->getSourceRange().getBegin().printToString(*Result.SourceManager) + " ";
          }
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(C->getSourceRange()), mutatedCode);
        }
      }
    }
}

void MutatorFrontendAction_17::MutatorASTConsumer_17::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxConstructorDecl().bind("Constructor");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}