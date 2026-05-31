//header file
#pragma once
#include "Mutator_base.h"

/**
 * StructuredBindingArrayDecomposition_476
 */ 
class MutatorFrontendAction_476 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(476)

private:
    class MutatorASTConsumer_476 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_476(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::VarDecl *> cur_arrays;
    };
};

//source file
#include "../include/StructuredBindingArrayDecomposition_476.h"

// ========================================================================================================
#define MUT476_OUTPUT 1

void MutatorFrontendAction_476::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("ArrayDecl")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == false)
      return;
    if (DL->getType()->isConstantArrayType() == false)
      return;
    if (DL->hasInit() == false)
      return;
    if (DL->getInitStyle() != VarDecl::InitializationStyle::CInit)
      return;
    if (DL->getInit()->getStmtClass() != Stmt::InitListExprClass)
      return;
    auto content =
        stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
    llvm::outs() << content << '\n';
    cur_arrays.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::DecompositionDecl>(
                 "DecompositionDecl")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    auto b = DL->bindings();
    if (b.size() != 2)
      return;
    auto content =
        stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
    llvm::outs() << content << '\n';
    auto type = DL->getType();
    if (type->isArrayType() == false)
      return;
    if (type->isConstantArrayType() == false)
      return;
    auto base = DL->getType()->getBaseElementTypeUnsafe();
    llvm::outs() << base.getAsString() << '\n';
    if (base->isStructureType() == false)
      return;
    auto init = DL->getInit();
    if (init->getStmtClass() != Stmt::DeclRefExprClass)
      return;
    auto ref = dyn_cast<DeclRefExpr>(init);
    auto var = ref->getDecl();
    llvm::outs() << var->getNameAsString() << '\n';
    bool flag = false;
    for (std::size_t i = 0; i < cur_arrays.size(); ++i) {
      if (cur_arrays[i] == var) {
        flag = true;
        break;
      }
    }
    if (flag == false)
      return;
    auto func = DL->getParentFunctionOrMethod();
    if (func == nullptr)
      return;
    if (func->isConstexpr() == false)
      return;
    auto func_name = func->getNameAsString();
    auto replace = "auto [a,b]=" + var->getNameAsString();
    Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()),
                        replace);
    auto static_assertion = "static_assert(!" + func_name + "(),\"\");\n";
    Rewrite.ReplaceText(func->getEndLoc(), 0, static_assertion);
  }
}
  
void MutatorFrontendAction_476::MutatorASTConsumer_476::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto array_matcher = varDecl().bind("ArrayDecl");
    auto decomposition_matcher = decompositionDecl().bind("DecompositionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(array_matcher, &callback);
    matchFinder.addMatcher(decomposition_matcher, &callback);
    matchFinder.matchAST(Context);
}