//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Constant_Initializer_325
 */ 
class MutatorFrontendAction_325 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(325)

private:
    class MutatorASTConsumer_325 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_325(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Constant_Initializer_325.h"

// ========================================================================================================
#define MUT325_OUTPUT 1

void MutatorFrontendAction_325::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("VarDecls")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      //Get the source code text of target node
      auto init = VD->getInit();
      if (!init)
        return;
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedInit = "/*mut325*/1 / 0"; // Replace with a division by zero
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(init->getSourceRange(), mutatedInit);
    }
}
  
void MutatorFrontendAction_325::MutatorASTConsumer_325::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(hasInitializer(integerLiteral())).bind("VarDecls");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}