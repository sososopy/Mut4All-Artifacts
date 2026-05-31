//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Default_Argument_With_Complex_Expression_161
 */ 
class MutatorFrontendAction_161 : public MutatorFrontendAction {
    public:
      MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(161)
    
    private:
      class MutatorASTConsumer_161 : public MutatorASTConsumer {
        public:
          MutatorASTConsumer_161(Rewriter &R) : TheRewriter(R) {}
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
          std::vector<const clang::FunctionDecl *> cur_functions;
      };
    };

//source file
#include "../include/replace_default_argument_with_complex_expression_161.h"

// ========================================================================================================
#define MUT161_OUTPUT 1

void MutatorFrontendAction_161::Callback::run(const MatchFinder::MatchResult &Result) {
  if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
    if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                   FD->getLocation()))
      return;
    if (FD->isOverloadedOperator())
      return;
    if (FD->isDefaulted())
      return;
    if (FD->isDeleted())
      return;
    if (FD->getNumParams() == 0)
      return;
    cur_functions.push_back(FD);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::ParmVarDecl>( "DefaultArgs")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (!DL->hasDefaultArg())
      return;
    auto expr = DL->getDefaultArg();
    if (expr == nullptr)
      return;
    auto expr_str = stringutils::rangetoStr(*(Result.SourceManager),
                                            expr->getSourceRange());
    llvm::outs() << "default args: " << expr_str << '\n';
    // llvm::outs()<<expr_str<<'\n';
    auto func = DL->getParentFunctionOrMethod();
    if (func == nullptr)
      return;
    auto func_name = func->getNameAsString();
    if (func_name == "")
      return;
    llvm::outs() << "func name: " << func_name << '\n';
    auto params = func->parameters();
    size_t index = 0;
    for (size_t i = 0; i < params.size(); ++i) {
      if (params[i] == DL) {
        index = i;
        break;
      }
    }
    auto param_name = DL->getNameAsString();
    if (param_name == "")
      param_name = "mut161";
    llvm::outs() << "param name: " << param_name << '\n';
    auto ins = func_name + "(" + param_name + ")";
    if (DL->getType()->isIntegralOrEnumerationType())
      ins = ins + "+0";
    else if (DL->getType()->isPointerType())
      ins = ins + "+nullptr";
    else if (DL->getType()->isFloatingType())
      ins = ins + "+0.0";
    else if (DL->getType()->isRecordType())
      ins = ins + "+decltype(" + param_name + ")()";
    else if (DL->getType()->isArrayType())
      ins = ins + "+decltype(" + param_name + ")()";
    else
      ins = ins + "+decltype(" + param_name + ")()";
    ins = "/*mut161*/" + ins;
    Rewrite.ReplaceText(expr->getSourceRange(), ins);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>( "Candidates")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->isDefaulted())
      return;
    if (DL->isDeleted())
      return;
    if (DL->getNumParams() == 0)
      return;
    if (DL->hasBody() == false)
      return;
    for (size_t i = 0; i < cur_functions.size(); ++i) {
      if (cur_functions[i] == DL)
        return;
    }
    auto name = DL->getNameAsString();
    if (name == "")
      return;
    auto params = DL->parameters();
    if (params.size() == 0)
      return;
    auto param_name = params[0]->getNameAsString();
    if (param_name == "")
      param_name = "mut161";
    string ins = "/*mut161*/auto mut161=" + name + "(" + param_name + ");\n";
    Rewrite.ReplaceText(DL->getBody()->getBeginLoc(), 0, ins);
  }
}

void MutatorFrontendAction_161::MutatorASTConsumer_161::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionDecl().bind("Functions");
    auto defaultarg_matcher = parmVarDecl(hasDefaultArgument()).bind("DefaultArgs");
    auto candidate_matcher = functionDecl().bind("Candidates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(defaultarg_matcher, &callback);
    matchFinder.addMatcher(candidate_matcher, &callback);
    matchFinder.matchAST(Context);
}