//header file
#pragma once
#include "Mutator_base.h"

/**
 * RequiresClauseLambdaReplace_114
 */ 
class MutatorFrontendAction_114 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(114)

private:
    class MutatorASTConsumer_114 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_114(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/RequiresClauseLambdaReplace_114.h"

// ========================================================================================================
#define MUT114_OUTPUT 1

void MutatorFrontendAction_114::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *DL = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambdas")) {
      //Filter nodes in header files
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getBeginLoc()))
        return;
      if (!DL->hasExplicitTemplateParams())
        return;
      auto DLtext = stringutils::rangetoStr(*(Result.SourceManager),
                                            DL->getSourceRange());
      llvm::outs() << DLtext << '\n';
      if (DLtext.find("requires") == string::npos)
        return;
      //Get the source code text of target node
      auto DLtext1 = DLtext.substr(DLtext.find("requires"));
      DLtext1 = DLtext1.substr(0, DLtext1.find(')'));
      llvm::outs() << DLtext1 << '\n';
      //Perform mutation on the source code text by applying string replacement
      DLtext1 = "(sizeof...(rest) > 0 && " + DLtext1 + ')';
      DLtext.replace(DLtext.find("requires"), DLtext1.length(), DLtext1);
      DLtext = "/*mut114*/" + DLtext;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(DL->getSourceRange(), DLtext);
    }
}
  
void MutatorFrontendAction_114::MutatorASTConsumer_114::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = lambdaExpr().bind("Lambdas");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}