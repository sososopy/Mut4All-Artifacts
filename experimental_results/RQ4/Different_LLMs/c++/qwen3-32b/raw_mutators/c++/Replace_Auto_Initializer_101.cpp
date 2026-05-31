//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Auto_Initializer_101
 */ 
class MutatorFrontendAction_101 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(101)

private:
    class MutatorASTConsumer_101 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_101(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Auto_Initializer_101.h"

// ========================================================================================================
#define MUT101_OUTPUT 1

void MutatorFrontendAction_101::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FieldDecl>("field")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->isStatic())
        return;
      //Get the source code text of target node
      std::string varName = FD->getNameAsString();
      std::string newDecl = "auto " + varName + " = value;";
      //Perform mutation on the source code text by applying string replacement
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(FD->getSourceRange(), newDecl);
    }
}
  
void MutatorFrontendAction_101::MutatorASTConsumer_101::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = fieldDecl(unless(isStatic()), hasParent(cxxRecordDecl())).bind("field");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}