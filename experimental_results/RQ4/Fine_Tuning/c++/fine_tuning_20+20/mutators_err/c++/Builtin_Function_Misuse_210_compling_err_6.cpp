//header file
#pragma once
#include "Mutator_base.h"

/**
 * Builtin_Function_Misuse_210
 */ 
class MutatorFrontendAction_210 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(210)

private:
    class MutatorASTConsumer_210 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_210(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::Expr *> exprs;
    };
};

//source file
#include "../include/Builtin_Function_Misuse_210.h"

// ========================================================================================================
#define MUT210_OUTPUT 1

void MutatorFrontendAction_210::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *BL = Result.Nodes.getNodeAs<clang::CallExpr>("BuiltinCall")) {
      if (!BL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     BL->getBeginLoc()))
        return;
      auto callee = BL->getDirectCallee();
      if (callee == nullptr)
        return;
      if (callee->getBuiltinID() == 0)
        return;
      auto args = BL->getArgs();
      for (auto arg : args) {
        exprs.push_back(arg);
      }
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::DeclRefExpr>("DeclRef")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getBeginLoc()))
        return;
      exprs.push_back(DL);
    } else if (auto *IL = Result.Nodes.getNodeAs<clang::IntegerLiteral>(
                   "IntegerLiteral")) {
      if (!IL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     IL->getBeginLoc()))
        return;
      exprs.push_back(IL);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CallExpr>("BuiltinCall")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getBeginLoc()))
        return;
      auto callee = DL->getDirectCallee();
      if (callee == nullptr)
        return;
      if (callee->getBuiltinID() == 0)
        return;
      auto args = DL->getArgs();
      if (args.empty())
        return;
      auto index = getrandom::getRandomIndex(args.size() - 1);
      auto target = args[index];
      auto target_type = target->getType();
      llvm::outs() << "target type: " << target_type.getAsString() << '\n';
      std::vector<const clang::Expr *> candidates;
      for (auto expr : exprs) {
        if (expr->getType() != target_type)
          candidates.push_back(expr);
      }
      if (candidates.empty()) {
        if (target_type->isPointerType()) {
          Rewrite.ReplaceText(target->getSourceRange(), "/*mut210*/0");
        } else if (target_type->isIntegerType()) {
          Rewrite.ReplaceText(target->getSourceRange(), "/*mut210*/nullptr");
        }
      } else {
        auto choice = getrandom::getRandomIndex(candidates.size() - 1);
        auto replacement = candidates[choice];
        auto replacement_code = stringutils::rangetoStr(
            *(Result.SourceManager), replacement->getSourceRange());
        Rewrite.ReplaceText(target->getSourceRange(),
                            "/*mut210*/" + replacement_code);
      }
    }
}

void MutatorFrontendAction_210::MutatorASTConsumer_210::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = callExpr().bind("BuiltinCall");
    auto declref_matcher = declRefExpr().bind("DeclRef");
    auto integer_matcher = integerLiteral().bind("IntegerLiteral");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(declref_matcher, &callback);
    matchFinder.addMatcher(integer_matcher, &callback);
    matchFinder.matchAST(Context);
    MatchFinder matchFinder2;
    matchFinder2.addMatcher(matcher, &callback);
    matchFinder2.matchAST(Context);
}