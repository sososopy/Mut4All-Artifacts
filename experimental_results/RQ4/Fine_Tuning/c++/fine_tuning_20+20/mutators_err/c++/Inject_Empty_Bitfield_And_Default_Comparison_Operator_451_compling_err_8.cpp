//header file
#pragma once
#include "Mutator_base.h"

/**
 * Inject_Empty_Bitfield_And_Default_Comparison_Operator_451
 */ 
class MutatorFrontendAction_451 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(451)

private:
    class MutatorASTConsumer_451 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_451(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        const clang::CXXRecordDecl *cur_class = nullptr;
    };
};

//source file
#include "../include/Inject_Empty_Bitfield_And_Default_Comparison_Operator_451.h"

// ========================================================================================================
#define MUT451_OUTPUT 1

void MutatorFrontendAction_451::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CL->getLocation()))
        return;
      if ((!CL->isStruct() && !CL->isClass()) || CL->isLambda())
        return;
      if (!CL->isCompleteDefinition())
        return;
      cur_class = CL;
    } else if (auto *OP =
                   Result.Nodes.getNodeAs<clang::CXXMethodDecl>("Operators")) {
      if (!OP || !Result.Context->getSourceManager().isWrittenInMainFile(
                     OP->getLocation()))
        return;
      if (cur_class == nullptr)
        return;
      if (OP->isImplicit() == false)
        return;
      if (OP->isOverloadedOperator() == false)
        return;
      if (OP->getOverloadedOperator() != OverloadedOperatorKind::OO_EqualEqual)
        return;
      if (OP->isDefaulted() == false)
        return;
      auto op_def = stringutils::rangetoStr(*(Result.SourceManager),
                                            OP->getSourceRange());
      llvm::outs() << op_def << '\n';
      auto bitfield = "int : 0;\n";
      bitfield = "/*mut451*/" + std::string(bitfield);
      Rewrite.ReplaceText(cur_class->getBeginLoc(), 0, bitfield);
    }
}

void MutatorFrontendAction_451::MutatorASTConsumer_451::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Classes");
    auto op_matcher = cxxMethodDecl().bind("Operators");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(op_matcher, &callback);
    matchFinder.matchAST(Context);
}