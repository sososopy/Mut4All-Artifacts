//header file
#pragma once
#include "Mutator_base.h"

/**
 * Enum_Forward_Declaration_Mutation_31
 */ 
class MutatorFrontendAction_31 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(31)

private:
    class MutatorASTConsumer_31 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_31(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Enum_Forward_Declaration_Mutation_31.h"

// ========================================================================================================
#define MUT31_OUTPUT 1

void MutatorFrontendAction_31::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::EnumDecl>("EnumDecl")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto name = MT->getNameAsString();
      auto underlyingType = MT->getIntegerType().getAsString();
      //Perform mutation on the source code text by applying string replacement
      string newEnumDecl = "enum " + name + " : " + underlyingType;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(MT->getSourceRange(), newEnumDecl);
    }
}
  
void MutatorFrontendAction_31::MutatorASTConsumer_31::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = enumDecl().bind("EnumDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}