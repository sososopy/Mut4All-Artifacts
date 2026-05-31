//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_atomic_type_comparison_42
 */ 
class MutatorFrontendAction_42 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(42)

private:
    class MutatorASTConsumer_42 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_42(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_atomic_type_comparison_42.h"

// ========================================================================================================
#define MUT42_OUTPUT 1

void MutatorFrontendAction_42::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *BO = Result.Nodes.getNodeAs<clang::BinaryOperator>("atomicComparison")) {
      //Filter nodes in header files
      if (!BO || !Result.Context->getSourceManager().isWrittenInMainFile(
                     BO->getExprLoc()))
        return;

      auto LHS = BO->getLHS()->IgnoreImpCasts();
      auto RHS = BO->getRHS()->IgnoreImpCasts();

      const auto *AtomicType = LHS->getType()->getAs<clang::AtomicType>();
      const auto *NonAtomicExpr = RHS;

      if (!AtomicType && (AtomicType = RHS->getType()->getAs<clang::AtomicType>())) {
        NonAtomicExpr = LHS;
      }

      if (AtomicType) {
        clang::QualType NonAtomicType = NonAtomicExpr->getType();
        clang::QualType IncompatibleType;

        if (NonAtomicType->isIntegerType()) {
          IncompatibleType = clang::QualType(clang::BuiltinType::Float, 0);
        } else if (NonAtomicType->isFloatingType()) {
          IncompatibleType = clang::QualType(clang::BuiltinType::Int, 0);
        } else {
          return;
        }

        std::string newTypeStr = IncompatibleType.getAsString();
        std::string originalExprText = std::string(clang::Lexer::getSourceText(
            clang::CharSourceRange::getTokenRange(NonAtomicExpr->getSourceRange()),
            *Result.SourceManager, clang::LangOptions(), 0));

        std::string mutatedExprText = "(" + newTypeStr + ")" + originalExprText;

        Rewrite.ReplaceText(NonAtomicExpr->getSourceRange(), mutatedExprText);
      }
    }
}
  
void MutatorFrontendAction_42::MutatorASTConsumer_42::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = binaryOperator(hasOperatorName("=="),
                                  hasEitherOperand(hasType(atomicType())))
                                  .bind("atomicComparison");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}