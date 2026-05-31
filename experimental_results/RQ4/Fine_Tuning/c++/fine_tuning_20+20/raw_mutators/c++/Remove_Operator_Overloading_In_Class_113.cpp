//header file
#pragma once
#include "Mutator_base.h"

/**
 * Remove_Operator_Overloading_In_Class_113
 */ 
class MutatorFrontendAction_113 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(113)

private:
    class MutatorASTConsumer_113 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_113(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/remove_operator_overloading_in_class_113.h"

// ========================================================================================================
#define MUT113_OUTPUT 1

void MutatorFrontendAction_113::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("Operator")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto op = stringutils::rangetoStr(*(Result.SourceManager),
                                        MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      op = "/*mut113*/";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), op);
    }
}
  
void MutatorFrontendAction_113::MutatorASTConsumer_113::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxMethodDecl(isOverloadedOperator()).bind("Operator");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}