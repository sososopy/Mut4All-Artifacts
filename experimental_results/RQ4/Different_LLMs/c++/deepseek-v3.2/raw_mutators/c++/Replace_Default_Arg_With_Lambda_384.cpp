//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Default_Arg_With_Lambda_384
 */ 
class MutatorFrontendAction_384 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(384)

private:
    class MutatorASTConsumer_384 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_384(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::ParmVarDecl*> candidateParams;
    };
};

//source file
#include "../include/Mutator_Replace_Default_Arg_With_Lambda_384.h"

// ========================================================================================================
#define MUT384_OUTPUT 1

void MutatorFrontendAction_384::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      //Record candidate parameters with default arguments
      for (unsigned i = 0; i < MT->getNumParams(); ++i) {
        const clang::ParmVarDecl* param = MT->getParamDecl(i);
        if (param->hasDefaultArg()) {
          candidateParams.push_back(param);
        }
      }
    }
    else if (auto *MT = Result.Nodes.getNodeAs<clang::ParmVarDecl>("ParmVarDecl")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Check if this parameter is in candidate list
      bool found = false;
      for (const clang::ParmVarDecl* param : candidateParams) {
        if (param == MT) {
          found = true;
          break;
        }
      }
      if (!found) return;
      //Get the source code text of target node
      SourceLocation defaultArgLoc = MT->getDefaultArg()->getBeginLoc();
      SourceLocation defaultArgEndLoc = MT->getDefaultArg()->getEndLoc();
      string paramName = MT->getNameAsString();
      //Perform mutation on the source code text by applying string replacement
      string lambdaExpr = "[] { return " + paramName + "; }";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(defaultArgLoc, defaultArgEndLoc), lambdaExpr);
      candidateParams.clear();
    }
}
  
void MutatorFrontendAction_384::MutatorASTConsumer_384::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher1 = functionDecl().bind("FunctionDecl");
    DeclarationMatcher matcher2 = parmVarDecl(hasDefaultArg()).bind("ParmVarDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher1, &callback);
    matchFinder.addMatcher(matcher2, &callback);
    matchFinder.matchAST(Context);
}