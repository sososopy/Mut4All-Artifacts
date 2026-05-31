//header file
#pragma once
#include "Mutator_base.h"

/**
 * Mutate_Type_Alias_Template_To_Struct_Definition_359
 */ 
class MutatorFrontendAction_359 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(359)

private:
    class MutatorASTConsumer_359 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_359(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutate_Type_Alias_Template_To_Struct_Definition_359.h"

// ========================================================================================================
#define MUT359_OUTPUT 1

void MutatorFrontendAction_359::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TypeAliasTemplateDecl>("typeAliasTemplate")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto aliasName = MT->getName();
      auto target = MT->getTemplatedDecl();
      auto aliasto = target->getUnderlyingType().getAsString();
      llvm::outs() << aliasName << ' ' << aliasto << '\n';
      //Perform mutation on the source code text by applying string replacement
      string struct_def = "struct " + aliasName + "{ " + aliasto +
                          " memberVar = 0; };";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(MT->getEndLoc(), 0, "/*mut359*/" + struct_def);
    }
}
  
void MutatorFrontendAction_359::MutatorASTConsumer_359::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = typeAliasTemplateDecl().bind("typeAliasTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}