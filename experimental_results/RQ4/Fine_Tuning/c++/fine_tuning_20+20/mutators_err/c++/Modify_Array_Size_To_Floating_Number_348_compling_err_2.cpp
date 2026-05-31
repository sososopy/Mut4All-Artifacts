//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Array_Size_To_Floating_Number_348
 */ 
class MutatorFrontendAction_348 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(348)

private:
    class MutatorASTConsumer_348 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_348(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_array_size_to_floating_number_348.h"

// ========================================================================================================
#define MUT348_OUTPUT 1

void MutatorFrontendAction_348::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *DL = Result.Nodes.getNodeAs<clang::ArrayType>("ArrayType")) {
      //Filter nodes in header files
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto DL_content = stringutils::rangetoStr(*(Result.SourceManager),
                                                DL->getSourceRange());
      llvm::outs() << DL_content << '\n';
      //Perform mutation on the source code text by applying string replacement
      DL_content += ".5";
      DL_content = "/*mut348*/" + DL_content;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(DL->getBeginLoc(), DL_content);
    }
}
  
void MutatorFrontendAction_348::MutatorASTConsumer_348::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    TypeMatcher matcher = arrayType().bind("ArrayType");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}