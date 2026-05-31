//header file
#pragma once
#include "Mutator_base.h"

/**
 * incomplete_union_array_initialization_646
 */ 
class MutatorFrontendAction_646 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(646)

private:
    class MutatorASTConsumer_646 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_646(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/incomplete_union_array_initialization_646.h"
#include "clang/ASTMatchers/ASTMatchers.h"

using namespace clang;
using namespace clang::ast_matchers;

// ========================================================================================================
#define MUT646_OUTPUT 1

void MutatorFrontendAction_646::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *UD = Result.Nodes.getNodeAs<clang::VarDecl>("UnionDecl")) {
      if (!UD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     UD->getLocation()))
        return;

      if (UD->getType()->isUnionType()) {
        auto InitList = dyn_cast<InitListExpr>(UD->getInit());
        if (InitList && InitList->getNumInits() > 0) {
          const Type *ElementType = InitList->getInit(0)->getType().getTypePtr();
          if (ElementType->isArrayType()) {
            std::string mutatedInit = "{ {} }";
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(UD->getSourceRange()), mutatedInit);
          }
        }
      }
    }
}
  
void MutatorFrontendAction_646::MutatorASTConsumer_646::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = varDecl(hasType(isUnionType())).bind("UnionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}