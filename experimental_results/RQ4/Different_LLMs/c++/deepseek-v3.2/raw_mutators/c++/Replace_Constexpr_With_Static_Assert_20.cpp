//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Constexpr_With_Static_Assert_20
 */ 
class MutatorFrontendAction_20 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(20)

private:
    class MutatorASTConsumer_20 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_20(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Constexpr_With_Static_Assert_20.h"

// ========================================================================================================
#define MUT20_OUTPUT 1

void MutatorFrontendAction_20::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("constexprVar")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      //Check if it's a constexpr variable declaration with an initializer
      if (!VD->isConstexpr() || !VD->hasInit())
        return;
      
      //Get the source code text of target node
      SourceManager &SM = *Result.SourceManager;
      SourceLocation StartLoc = VD->getBeginLoc();
      SourceLocation EndLoc = VD->getEndLoc();
      
      //Get the variable name and initializer as strings
      std::string VarName = VD->getNameAsString();
      std::string InitStr = stringutils::rangetoStr(SM, VD->getInit()->getSourceRange());
      
      //Perform mutation on the source code text by applying string replacement
      std::string StaticAssert = " static_assert(" + VarName + " == " + InitStr + ", \"\");";
      
      //Replace the original AST node with the mutated one
      Rewrite.InsertTextAfterToken(EndLoc, StaticAssert);
    }
}
  
void MutatorFrontendAction_20::MutatorASTConsumer_20::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(isConstexpr()).bind("constexprVar");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}