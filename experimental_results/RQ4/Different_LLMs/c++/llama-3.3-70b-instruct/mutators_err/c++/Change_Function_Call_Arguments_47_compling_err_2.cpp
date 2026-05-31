//header file
#pragma once
#include "Mutator_base.h"

/**
 * Change_Function_Call_Arguments_47
 */ 
class MutatorFrontendAction_47 : public MutatorFrontendAction {
public:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef InFile) override {
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        return std::make_unique<MutatorASTConsumer_47>(TheRewriter);
    }

private:
    class MutatorASTConsumer_47 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_47(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
    Rewriter TheRewriter;
};

//source file
#include "../include/Mutator_Change_Function_Call_Arguments_47.h"

// ========================================================================================================
#define MUT47_OUTPUT 1

void MutatorFrontendAction_47::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FC = Result.Nodes.getNodeAs<clang::CallExpr>("FunctionCalls")) {
      //Filter nodes in header files
      if (!FC || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FC->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto args = FC->getArgs();
      if (args.size() < 2) return;
      //Perform mutation on the source code text by applying string replacement
      std::string argStr;
      for (unsigned i = 0; i < args.size(); ++i) {
        if (i > 0) argStr += ", ";
        argStr += "/*mut47*/" + args[i]->getType().getAsString();
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FC->getSourceRange()), "/*mut47*/" + FC->getDirectCallee()->getNameAsString() + "(" + argStr + ")");
    }
}
  
void MutatorFrontendAction_47::MutatorASTConsumer_47::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    CallExprMatcher matcher = callExpr().bind("FunctionCalls");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}