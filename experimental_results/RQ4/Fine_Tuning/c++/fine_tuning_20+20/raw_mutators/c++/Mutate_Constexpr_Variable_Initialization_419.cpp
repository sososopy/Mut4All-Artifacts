//header file
#pragma once
#include "Mutator_base.h"

/**
 * Mutate_Constexpr_Variable_Initialization_419
 */ 
class MutatorFrontendAction_419 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(419)

private:
    class MutatorASTConsumer_419 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_419(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutate_Constexpr_Variable_Initialization_419.h"

// ========================================================================================================
#define MUT419_OUTPUT 1

void MutatorFrontendAction_419::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("ConstexprVariables")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (!DL->isConstexpr())
        return;
      if (DL->hasInit() == false)
        return;
      auto init = DL->getInit();
      auto init_str = stringutils::rangetoStr(*(Result.SourceManager),
                                              init->getSourceRange());
      auto new_init = "(true ? " + init_str + " : 0)";
      llvm::outs() << new_init << '\n';
      Rewrite.ReplaceText(init->getSourceRange(), new_init);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>(
                   "NonConstexprVariables")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isConstexpr())
        return;
      if (DL->hasInit() == false)
        return;
      auto init = DL->getInit();
      auto init_str = stringutils::rangetoStr(*(Result.SourceManager),
                                              init->getSourceRange());
      auto new_decl = "constexpr auto " + DL->getNameAsString() +
                      " = (true ? " + init_str + " : 0)";
      llvm::outs() << new_decl << '\n';
      Rewrite.ReplaceText(DL->getSourceRange(), new_decl);
    }
}
  
void MutatorFrontendAction_419::MutatorASTConsumer_419::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto constexpr_var_matcher = varDecl(isExpansionInMainFile()).bind("ConstexprVariables");
    auto non_constexpr_var_matcher = varDecl(isExpansionInMainFile()).bind("NonConstexprVariables");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(constexpr_var_matcher, &callback);
    matchFinder.addMatcher(non_constexpr_var_matcher, &callback);
    matchFinder.matchAST(Context);
}