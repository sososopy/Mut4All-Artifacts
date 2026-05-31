//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Call_With_Pack_Decltype_278
 */ 
class MutatorFrontendAction_278 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(278)

private:
    class MutatorASTConsumer_278 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_278(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_.h"

// ========================================================================================================
#define MUT278_OUTPUT 1

void MutatorFrontendAction_278::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CallExpr>("CallExpr")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      const FunctionDecl *FD = MT->getDirectCallee();
      if (!FD) return;
      const DeclContext *DC = MT->getParent()->getDeclContext();
      if (!DC) return;
      const FunctionDecl *ParentFD = dyn_cast<FunctionDecl>(DC);
      if (!ParentFD) return;
      if (ParentFD->getNumParams() == 0) return;
      
      //Select a parameter randomly
      int paramIndex = getrandom::getRandomIndex(ParentFD->getNumParams() - 1);
      const ParmVarDecl *Param = ParentFD->getParamDecl(paramIndex);
      std::string paramName = Param->getNameAsString();
      
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedText = "decltype(" + paramName + ")...";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedText);
    }
}
  
void MutatorFrontendAction_278::MutatorASTConsumer_278::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = callExpr(hasParent(compoundStmt()), hasAncestor(functionDecl(hasBody(compoundStmt())))).bind("CallExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}