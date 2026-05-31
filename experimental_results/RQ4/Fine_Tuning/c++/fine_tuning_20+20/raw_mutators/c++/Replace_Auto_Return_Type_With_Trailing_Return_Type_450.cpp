//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Auto_Return_Type_With_Trailing_Return_Type_450
 */ 
class MutatorFrontendAction_450 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(450)

private:
    class MutatorASTConsumer_450 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_450(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result);
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Replace_Auto_Return_Type_With_Trailing_Return_Type_450.h"

// ========================================================================================================
#define MUT450_OUTPUT 1

void MutatorFrontendAction_450::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (!FD->getReturnType().getAsString().starts_with("auto"))
        return;
      if (!FD->hasTrailingReturnType())
        return;
      //Get the source code text of target node
      auto ret = stringutils::rangetoStr(*(Result.SourceManager),
                                         FD->getReturnTypeSourceRange());
      auto trailing = stringutils::rangetoStr(*(Result.SourceManager),
                                              FD->getTrailingReturnTypeSourceRange());
      //Perform mutation on the source code text by applying string replacement
      trailing = trailing.substr(trailing.find("->") + 2);
      llvm::outs() << trailing << '\n';
      auto new_ret = "/*mut450*/" + trailing;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(FD->getReturnTypeSourceRange(), new_ret);
    }
}
  
void MutatorFrontendAction_450::MutatorASTConsumer_450::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl().bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}