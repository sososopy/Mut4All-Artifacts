//header file
#pragma once
#include "Mutator_base.h"

/**
 * Constant_Array_Initialization_With_Incomplete_Type_299
 */ 
class MutatorFrontendAction_299 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(299)

private:
    class MutatorASTConsumer_299 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_299(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Constant_Array_Initialization_With_Incomplete_Type_299.h"

// ========================================================================================================
#define MUT299_OUTPUT 1

void MutatorFrontendAction_299::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("ConstArrayDecl")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (!DL->getType()->isConstantArrayType())
        return;
      if (!DL->hasInit())
        return;
      auto init = DL->getInit();
      if (!init->isConstantInitializer(*Result.Context, false))
        return;
      auto init_text = stringutils::rangetoStr(*(Result.SourceManager),
                                               init->getSourceRange());
      auto new_init_text = init_text;
      if (init_text.size() >= 3) {
        new_init_text.pop_back();
        new_init_text.pop_back();
        new_init_text += ",a}";
      }
      llvm::outs() << new_init_text;
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(init->getSourceRange()), new_init_text);
    }
}
  
void MutatorFrontendAction_299::MutatorASTConsumer_299::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = varDecl(hasType(arrayType(hasElementType(isIncompleteType())))).bind("ConstArrayDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}