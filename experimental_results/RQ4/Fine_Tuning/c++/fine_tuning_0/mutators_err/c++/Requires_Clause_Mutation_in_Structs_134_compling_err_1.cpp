//header file
#pragma once
#include "Mutator_base.h"

/**
 * Requires_Clause_Mutation_in_Structs_134
 */ 
class MutatorFrontendAction_134 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(134)

private:
    class MutatorASTConsumer_134 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_134(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Requires_Clause_Mutation_in_Structs_134.h"

// ========================================================================================================
#define MUT134_OUTPUT 1

void MutatorFrontendAction_134::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ST = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Struct")) {
      if (!ST || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ST->getLocation()))
        return;

      if (!ST->isStruct())
        return;

      for (auto ctor : ST->ctors()) {
        if (ctor->isImplicit())
          continue;

        if (ctor->getNumParams() == 0)
          continue;

        if (!ctor->getTrailingRequiresClause())
          continue;

        auto requires_expr =
            stringutils::rangetoStr(*(Result.SourceManager),
                                    ctor->getTrailingRequiresClause()
                                        ->getConstraintExpr()
                                        ->getSourceRange());
        requires_expr = "requires (" + requires_expr + ")";
        llvm::outs() << requires_expr << '\n';
        Rewrite.ReplaceText(ctor->getTrailingRequiresClause()
                                ->getConstraintExpr()
                                ->getSourceRange(),
                            requires_expr);
      }
    }
}
  
void MutatorFrontendAction_134::MutatorASTConsumer_134::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl().bind("Struct");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}