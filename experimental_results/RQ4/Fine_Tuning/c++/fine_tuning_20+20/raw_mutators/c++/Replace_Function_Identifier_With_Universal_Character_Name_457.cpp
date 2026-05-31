//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Function_Identifier_With_Universal_Character_Name_457
 */ 
class MutatorFrontendAction_457 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(457)

private:
    class MutatorASTConsumer_457 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_457(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Function_Identifier_With_Universal_Character_Name_457.h"

// ========================================================================================================
#define MUT457_OUTPUT 1

void MutatorFrontendAction_457::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Get the source code text of target node
      auto identifier = FD->getNameAsString();
      llvm::outs() << identifier << '\n';
      //(optional)Record the node information to be used in the mutation process
      if (identifier.find("\\u") != string::npos)
        return;
      //Perform mutation on the source code text by applying string replacement
      string result = "";
      for (char c : identifier) {
        if (c >= 32 && c <= 126) {
          result += "\\u00";
          result += c < 16 ? "0" : "";
          result += llvm::utohexstr(c);
        } else
          result += c;
      }
      llvm::outs() << result << '\n';
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(FD->getLocation(), identifier.length(), result);
    }
}
  
void MutatorFrontendAction_457::MutatorASTConsumer_457::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl().bind("FunctionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}