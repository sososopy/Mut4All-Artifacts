//header file
#pragma once
#include "Mutator_base.h"

/**
 * Using_Enum_Value_With_Bitwise_OR_And_Parentheses_83
 */ 
class MutatorFrontendAction_83 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(83)

private:
    class MutatorASTConsumer_83 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_83(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/using_enum_value_with_bitwise_or_and_parentheses_83.h"

// ========================================================================================================
#define MUT83_OUTPUT 1

void MutatorFrontendAction_83::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::DeclRefExpr>("DeclRefExpr")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getBeginLoc()))
        return;
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      llvm::outs() << content << '\n';
      if (DL->getDecl()->getKind() != Decl::Kind::EnumConstant)
        return;
      if (DL->getType().getAsString().find("::") == string::npos)
        return;
      if (DL->getType().getAsString().find("enum ") != string::npos)
        return;
      if (DL->getType().getAsString().find("enum class ") != string::npos)
        return;
      auto parent = DL->getParent();
      if (parent == nullptr)
        return;
      if (parent->getStmtClass() != Stmt::StmtClass::BinaryOperatorClass)
        return;
      auto bop = cast<BinaryOperator>(parent);
      if (bop->getOpcode() != BinaryOperatorKind::BO_Or &&
          bop->getOpcode() != BinaryOperatorKind::BO_And &&
          bop->getOpcode() != BinaryOperatorKind::BO_Xor)
        return;
      auto bop_content = stringutils::rangetoStr(*(Result.SourceManager),
                                                 bop->getSourceRange());
      bop_content = "/*mut83*/(" + bop_content + ")";
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(bop->getSourceRange()),
                          bop_content);
    }
}
  
void MutatorFrontendAction_83::MutatorASTConsumer_83::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = declRefExpr().bind("DeclRefExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}