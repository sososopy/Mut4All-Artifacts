//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Sizeof_With_Variable_499
 */ 
class MutatorFrontendAction_499 : public MutatorFrontendAction {
public:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef InFile) override {
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        return std::make_unique<MutatorASTConsumer_499>(TheRewriter);
    }

private:
    class MutatorASTConsumer_499 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_499(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
    Rewriter TheRewriter;
};

//source file
#include "../include/Replace_Sizeof_With_Variable_499.h"
#include "clang/AST/ASTContext.h"
#include "clang/Basic/SourceManager.h"
#include "llvm/Support/SourceMgr.h"

// ========================================================================================================
#define MUT499_OUTPUT 1

void MutatorFrontendAction_499::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::UnaryOperator>("SizeofOperator")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto SM = Result.SourceManager;
      auto range = MT->getSourceRange();
      std::string declaration = llvm::StringRef(SM->getBuffer(SM->getBufferID(range.getBegin()), range).getBuffer()).str();
      //Perform mutation on the source code text by applying string replacement
      std::string replacement = "int var = " + declaration + ";";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), replacement);
    }
}
  
void MutatorFrontendAction_499::MutatorASTConsumer_499::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = unaryOperator(hasOperatorName("sizeof")).bind("SizeofOperator");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}