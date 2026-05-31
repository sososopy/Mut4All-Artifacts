//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Rewrite/Core/Rewriter.h"

using namespace clang;
using namespace clang::ast_matchers;

/**
 * modify_template_usage_in_pseudo_destructor_expressions_16
 */ 
class MutatorFrontendAction_16 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(16)

private:
    class MutatorASTConsumer_16 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_16(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_template_usage_in_pseudo_destructor_expressions_16.h"

// ========================================================================================================
#define MUT16_OUTPUT 1

void MutatorFrontendAction_16::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *Expr = Result.Nodes.getNodeAs<clang::CXXPseudoDestructorExpr>("PseudoDestructor")) {
      if (!Expr || !Result.Context->getSourceManager().isWrittenInMainFile(
                     Expr->getBeginLoc()))
        return;
      
      if (const auto *Type = Expr->getDestroyedType().getTypePtrOrNull()) {
        auto TypeStr = Type->getCanonicalTypeInternal().getAsString();
        auto Range = CharSourceRange::getTokenRange(Expr->getSourceRange());
        std::string ModifiedStr = "(*ptr).~" + TypeStr + "...();";
        Rewrite.ReplaceText(Range, ModifiedStr);
      }
    }
}
  
void MutatorFrontendAction_16::MutatorASTConsumer_16::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxPseudoDestructorExpr().bind("PseudoDestructor");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}