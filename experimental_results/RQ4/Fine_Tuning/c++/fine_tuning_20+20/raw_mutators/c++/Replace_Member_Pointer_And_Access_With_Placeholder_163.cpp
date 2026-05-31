//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Member_Pointer_And_Access_With_Placeholder_163
 */ 
class MutatorFrontendAction_163 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(163)

private:
    class MutatorASTConsumer_163 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_163(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::MemberPointerType *> cur_member_pointer;
        std::vector<const clang::MemberExpr *> cur_member_expr;
    };
};

//source file
#include "../include/replace_member_pointer_and_access_with_placeholder_163.h"

// ========================================================================================================
#define MUT163_OUTPUT 1

void MutatorFrontendAction_163::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::MemberPointerType>("MemPtr")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      cur_member_pointer.push_back(MT);
    } else if (auto *ME =
                   Result.Nodes.getNodeAs<clang::MemberExpr>("MemExpr")) {
      if (!ME || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ME->getBeginLoc()))
        return;
      cur_member_expr.push_back(ME);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::DeclRefExpr>("Decl")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getBeginLoc()))
        return;
      auto DLstr = stringutils::rangetoStr(*(Result.SourceManager),
                                           DL->getSourceRange());
      DLstr = "decltype(nullptr)(" + DLstr + ")";
      Rewrite.ReplaceText(DL->getSourceRange(), DLstr);
    }
  }

void MutatorFrontendAction_163::MutatorASTConsumer_163::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto member_pointer_matcher = qualType(memberPointerType()).bind("MemPtr");
    auto member_expr_matcher = memberExpr().bind("MemExpr");
    auto declref_matcher = declRefExpr().bind("Decl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(member_pointer_matcher, &callback);
    matchFinder.addMatcher(member_expr_matcher, &callback);
    matchFinder.addMatcher(declref_matcher, &callback);
    matchFinder.matchAST(Context);
    for (auto *MT : callback.cur_member_pointer) {
      auto DL = MT->getBeginLoc();
      auto DLstr = stringutils::rangetoStr(*(Context.getSourceManager()),
                                           MT->getBeginLoc(),
                                           MT->getEndLoc());
      DLstr = "decltype(nullptr)";
      llvm::outs() << DLstr << '\n';
      TheRewriter.ReplaceText(MT->getBeginLoc(), DLstr);
    }
    for (auto *ME : callback.cur_member_expr) {
      auto DLstr = stringutils::rangetoStr(*(Context.getSourceManager()),
                                           ME->getSourceRange());
      DLstr = "decltype(nullptr)(" + DLstr + ")";
      TheRewriter.ReplaceText(ME->getSourceRange(), DLstr);
    }
}