//header file
#pragma once
#include "Mutator_base.h"

/**
 * Invalid_Reference_Inside_Constexpr_Function_7
 */ 
class MutatorFrontendAction_7 : public MutatorFrontendAction {
    public:
      MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(7)
    
    private:
      class MutatorASTConsumer_7 : public MutatorASTConsumer {
        public:
          MutatorASTConsumer_7(Rewriter &R) : TheRewriter(R) {}
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
          std::vector<const clang::VarDecl *> non_local_vars;
          std::vector<const clang::CXXRecordDecl *> cur_classes;
      };
    };

//source file
#include "../include/invalid_reference_inside_constexpr_function_7.h"

// ========================================================================================================
#define MUT7_OUTPUT 1

void MutatorFrontendAction_7::Callback::run(const MatchFinder::MatchResult &Result) {
  if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isCompleteDefinition() == false)
      return;
    cur_classes.push_back(DL);
    auto decls = DL->decls();
    for (auto decl : decls) {
      if (auto func = dyn_cast<FunctionDecl>(decl)) {
        if (func->isConstexpr() == true)
          return;
      }
    }
    Rewrite.ReplaceText(DL->getEndLoc(), 0,
                        "\n/*mut7*/constexpr int mut7() {return 0;}\n");
  } else if (auto *FD =
                 Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
    if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                   FD->getLocation()))
      return;
    if (FD->isConstexpr() == false)
      return;
    if (FD->isInlined() == false)
      return;
    if (FD->hasBody() == false)
      return;
    if (FD->getReturnType().getAsString() != "int")
      return;
    auto DL = FD->getParent();
    if (DL->isStruct() == false && DL->isClass() == false)
      return;
    auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                           FD->getSourceRange());
    if (non_local_vars.empty() == true)
      return;
    size_t index = getrandom::getRandomIndex(non_local_vars.size() - 1);
    auto target = non_local_vars[index];
    content.insert(content.find('{') + 1, "/*mut7*/return " + target->getNameAsString() + ";\n");
    Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()),
                        content);
  } else if (auto *VL = Result.Nodes.getNodeAs<clang::VarDecl>("Variables")) {
    if (!VL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   VL->getLocation()))
      return;
    auto DL = VL->getParentFunctionOrMethod();
    if (DL != nullptr)
      return;
    non_local_vars.push_back(VL);
  }
}

void MutatorFrontendAction_7::MutatorASTConsumer_7::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Classes");
    auto func_matcher = functionDecl().bind("Functions");
    auto var_matcher = varDecl().bind("Variables");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(func_matcher, &callback);
    matchFinder.addMatcher(var_matcher, &callback);
    matchFinder.matchAST(Context);
}