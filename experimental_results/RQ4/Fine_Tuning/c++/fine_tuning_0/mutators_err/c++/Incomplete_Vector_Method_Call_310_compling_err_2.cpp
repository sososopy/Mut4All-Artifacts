//header file
#pragma once
#include "Mutator_base.h"

/**
 * Incomplete_Vector_Method_Call_310
 */ 
class MutatorFrontendAction_310 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(310)

private:
    class MutatorASTConsumer_310 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_310(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/incomplete_vector_method_call_310.h"

// ========================================================================================================
#define MUT310_OUTPUT 1

void MutatorFrontendAction_310::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vectors")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (!VD->hasInit())
        return;
      auto init = VD->getInit();
      if (!init->getType()->isRecordType())
        return;
      auto name = stringutils::rangetoStr(*(Result.SourceManager),
                                          VD->getName().getSourceRange());
      auto type = stringutils::rangetoStr(*(Result.SourceManager),
                                          VD->getTypeSourceInfo()->getTypeLoc().getSourceRange());
      if (type.find("vector") == string::npos)
        return;
      auto expr = stringutils::rangetoStr(*(Result.SourceManager),
                                          init->getSourceRange());
      auto decl = type + " " + name + " = " + expr;
      llvm::outs() << decl << '\n';
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(VD->getSourceRange()),
                          decl);
    } else if (auto *MD =
                   Result.Nodes.getNodeAs<clang::MemberExpr>("MemberExprs")) {
      if (!MD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MD->getBeginLoc()))
        return;
      if (!MD->getType()->isDependentType())
        return;
      auto base = MD->getBase();
      auto base_str = stringutils::rangetoStr(*(Result.SourceManager),
                                              base->getSourceRange());
      auto member_str = stringutils::rangetoStr(*(Result.SourceManager),
                                                MD->getMemberNameInfo().getSourceRange());
      llvm::outs() << base_str << " " << member_str << '\n';
      Rewrite.ReplaceText(MD->getSourceRange(), base_str + ".");
    }
  }
  
void MutatorFrontendAction_310::MutatorASTConsumer_310::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = varDecl().bind("Vectors");
    auto memberexpr_matcher = memberExpr().bind("MemberExprs");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(memberexpr_matcher, &callback);
    matchFinder.matchAST(Context);
}