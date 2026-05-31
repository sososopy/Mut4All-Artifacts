//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Function_Pointer_Assignment_242
 */ 
class MutatorFrontendAction_242 : public MutatorFrontendAction {
public:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef InFile) override {
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        return std::make_unique<MutatorASTConsumer_242>(TheRewriter);
    }

private:
    class MutatorASTConsumer_242 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_242(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Function_Pointer_Assignment_242.h"

// ========================================================================================================
#define MUT242_OUTPUT 1

void MutatorFrontendAction_242::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::VarDecl>("VarDecl")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      // Check if the variable is declared with decltype(auto)
      if (MT->getType()->isDecltypeType()) {
        // Check if the initializer is a function pointer
        if (auto *init = MT->getInit()) {
          if (init->getType()->isPointerType()) {
            if (auto pointeeType = dyn_cast<clang::PointerType>(init->getType())->getPointeeType()) {
              if (pointeeType->isFunctionPointerType()) {
                // Perform mutation on the source code text by applying string replacement
                // Replace decltype(auto) with auto
                declaration.replace(declaration.find("decltype(auto)"), 13, "auto");
                //Replace the original AST node with the mutated one
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
              }
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_242::MutatorASTConsumer_242::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(hasType(decltypeType())).bind("VarDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}