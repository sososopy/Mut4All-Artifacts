//header file
#pragma once
#include "Mutator_base.h"

/**
 * Transform_Nested_Array_Initialization_Into_Lambda_42
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
        std::vector<const clang::VarDecl *> cur_vars;
    };
};

//source file
#include "../include/transform_nested_array_initialization_into_lambda_42.h"

// ========================================================================================================
#define MUT42_OUTPUT 1

void MutatorFrontendAction_42::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Arrays")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->getType()->isArrayType() == false)
        return;
      if (VD->hasInit() == false)
        return;
      auto init = VD->getInit();
      if (init->getStmtClass() != Stmt::StmtClass::InitListExprClass)
        return;
      auto initlist = dyn_cast<InitListExpr>(init);
      if (initlist->hasArrayFiller() == true)
        return;
      if (initlist->getNumInits() == 0)
        return;
      auto first_init = initlist->getInit(0);
      if (first_init->getStmtClass() != Stmt::StmtClass::InitListExprClass)
        return;
      cur_vars.push_back(VD);
    } else if (auto *MD = Result.Nodes.getNodeAs<clang::CXXMethodDecl>(
                   "Methods")) {
      if (!MD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MD->getLocation()))
        return;

      if (MD->hasBody() && !MD->isVirtual()) {
        auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MD->getSourceRange());
        llvm::outs() << "mut42: " << declaration << '\n';
        for (auto var : cur_vars) {
          auto varname = var->getNameAsString();
          auto pos = declaration.find(varname);
          if (pos != string::npos) {
            declaration.insert(pos, "/*mut42*/");
            declaration.insert(declaration.find('=', pos) + 1, "[&](){");
            declaration.insert(declaration.find(';', pos), "}()");
          }
        }
        int choice = getrandom::getRandomIndex(1);
        if (choice) {
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(MD->getSourceRange()), declaration);
        }
      }
    }
}
  
void MutatorFrontendAction_42::MutatorASTConsumer_42::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto array_matcher = varDecl().bind("Arrays");
    auto method_matcher = cxxMethodDecl(unless(cxxConstructorDecl())).bind("Methods");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(array_matcher, &callback);
    matchFinder.addMatcher(method_matcher, &callback);
    matchFinder.matchAST(Context);
}