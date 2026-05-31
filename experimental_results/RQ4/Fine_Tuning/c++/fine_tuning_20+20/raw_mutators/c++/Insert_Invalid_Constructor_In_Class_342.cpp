//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Invalid_Constructor_In_Class_342
 */ 
class MutatorFrontendAction_342 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(342)

private:
    class MutatorASTConsumer_342 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_342(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Insert_Invalid_Constructor_In_Class_342.h"

// ========================================================================================================
#define MUT342_OUTPUT 1

void MutatorFrontendAction_342::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      //Filter nodes in header files
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      //Get the source code text of target node
      auto classname = DL->getNameAsString();
      //Perform mutation on the source code text by applying string replacement
      auto ins = "/*mut342*/" + classname +
                 "(mut342_invalid_type arg1){mut342_invalid_code;}";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(DL->getEndLoc(), 0, ins);
    }
}
  
void MutatorFrontendAction_342::MutatorASTConsumer_342::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}