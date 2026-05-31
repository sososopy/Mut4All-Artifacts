//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Base_Class_Pointer_With_Derived_190
 */ 
class MutatorFrontendAction_190 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(190)

private:
    class MutatorASTConsumer_190 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_190(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Base_Class_Pointer_With_Derived_190.h"

// ========================================================================================================
#define MUT190_OUTPUT 1

void MutatorFrontendAction_190::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *BD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Bases")) {
      if (!BD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     BD->getLocation()))
        return;
      if (!BD->isCompleteDefinition())
        return;
      auto deriveds = BD->derived_begin();
      if (deriveds == BD->derived_end())
        return;
      auto derived = *deriveds;
      auto derived_name = derived->getNameAsString();
      auto base_name = BD->getNameAsString();
      llvm::outs() << derived_name << '\n';
      if (auto *DL = Result.Nodes.getNodeAs<clang::DeclRefExpr>("Decls")) {
        if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                       DL->getLocation()))
          return;
        if (DL->getType().getAsString() == base_name) {
          Rewrite.ReplaceText(DL->getBeginLoc(), base_name.length(),
                              derived_name);
        }
      }
    }
}
  
void MutatorFrontendAction_190::MutatorASTConsumer_190::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto base_matcher = cxxRecordDecl().bind("Bases");
    auto decl_matcher = declRefExpr().bind("Decls");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(base_matcher, &callback);
    matchFinder.addMatcher(decl_matcher, &callback);
    matchFinder.matchAST(Context);
}