//header file
#pragma once
#include "Mutator_base.h"

/**
 * Transform_Explicit_Lambda_To_Generic_492
 */ 
class MutatorFrontendAction_492 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(492)

private:
    class MutatorASTConsumer_492 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_492(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Transform_Explicit_Lambda_To_Generic_492.h"

// ========================================================================================================
#define MUT492_OUTPUT 1

void MutatorFrontendAction_492::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambda")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto lambda = stringutils::rangetoStr(*(Result.SourceManager),
                                            MT->getSourceRange());
      if (MT->hasExplicitParameters() == false)
        return;
      llvm::outs() << lambda << '\n';
      auto capture = stringutils::rangetoStr(*(Result.SourceManager),
                                             MT->getCaptureDefaultLoc());
      auto params = stringutils::rangetoStr(*(Result.SourceManager),
                                            MT->getIntroducerRange());
      llvm::outs() << params << '\n';
      if (params.find('&') == string::npos)
        return;
      auto body = stringutils::rangetoStr(*(Result.SourceManager),
                                          MT->getBody()->getSourceRange());
      llvm::outs() << body << '\n';
      //Perform mutation on the source code text by applying string replacement
      string new_lambda = capture + "[&](this auto self)" + body;
      llvm::outs() << new_lambda << '\n';
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), new_lambda);
    }
}
  
void MutatorFrontendAction_492::MutatorASTConsumer_492::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = lambdaExpr().bind("Lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}