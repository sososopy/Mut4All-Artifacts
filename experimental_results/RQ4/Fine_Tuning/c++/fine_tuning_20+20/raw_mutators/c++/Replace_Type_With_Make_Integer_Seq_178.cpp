//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Type_With_Make_Integer_Seq_178
 */ 
class MutatorFrontendAction_178 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(178)

private:
    class MutatorASTConsumer_178 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_178(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Type_With_Make_Integer_Seq_178.h"

// ========================================================================================================
#define MUT178_OUTPUT 1

void MutatorFrontendAction_178::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::Type>("Type")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto type = stringutils::rangetoStr(*(Result.SourceManager),
                                          MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      if (type.find("__make_integer_seq") == string::npos)
        type = "__make_integer_seq";
      else
        return;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(MT->getBeginLoc(), type.size(), type);
    }
}
  
void MutatorFrontendAction_178::MutatorASTConsumer_178::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = type().bind("Type");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}