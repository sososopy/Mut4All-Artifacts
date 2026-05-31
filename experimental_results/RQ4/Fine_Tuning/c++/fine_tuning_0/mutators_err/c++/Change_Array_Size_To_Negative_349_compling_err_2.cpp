//header file
#pragma once
#include "Mutator_base.h"

/**
 * Change_Array_Size_To_Negative_349
 */ 
class MutatorFrontendAction_349 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(349)

private:
    class MutatorASTConsumer_349 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_349(Rewriter &R) : TheRewriter(R) {}
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
        
    };
};

//source file
#include "../include/Change_Array_Size_To_Negative_349.h"

// ========================================================================================================
#define MUT349_OUTPUT 1

void MutatorFrontendAction_349::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::ArrayType>("Arrays")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getBeginLoc()))
        return;

      auto size = DL->getSize();
      if (size == nullptr)
        return;

      auto size_str = stringutils::rangetoStr(*(Result.SourceManager),
                                              size->getSourceRange());
      size_str = "-(" + size_str + ")";
      llvm::outs() << size_str << '\n';
      Rewrite.ReplaceText(size->getSourceRange(), size_str);
    }
}
  
void MutatorFrontendAction_349::MutatorASTConsumer_349::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = arrayType().bind("Arrays");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}