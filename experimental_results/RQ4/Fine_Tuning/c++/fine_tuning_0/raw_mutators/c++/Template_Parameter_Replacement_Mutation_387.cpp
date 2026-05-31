//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Parameter_Replacement_Mutation_387
 */ 
class MutatorFrontendAction_387 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(387)

private:
    class MutatorASTConsumer_387 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_387(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Template_Parameter_Replacement_Mutation_387.h"

// ========================================================================================================
#define MUT387_OUTPUT 1

void MutatorFrontendAction_387::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::TemplateDecl>("Templates")) {
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;
      if (TD->isImplicit())
        return;
      auto TL = TD->getTemplateParameters();
      if (TL == nullptr)
        return;
      auto TL_str = stringutils::rangetoStr(*(Result.SourceManager),
                                            TL->getSourceRange());
      llvm::outs() << TL_str << '\n';
      auto TL_size = TL->size();
      if (TL_size == 0)
        return;
      int index = getrandom::getRandomIndex(TL_size - 1);
      auto TP = TL->getParam(index);
      if (TP == nullptr)
        return;
      auto TP_str = stringutils::rangetoStr(*(Result.SourceManager),
                                            TP->getSourceRange());
      llvm::outs() << TP_str << '\n';
      string replace_str = "U";
      TL_str = stringutils::replace_str(TL_str, TP_str, replace_str);
      llvm::outs() << TL_str << '\n';
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(TL->getSourceRange()),
                          TL_str);
    }
}
  
void MutatorFrontendAction_387::MutatorASTConsumer_387::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = templateDecl().bind("Templates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}