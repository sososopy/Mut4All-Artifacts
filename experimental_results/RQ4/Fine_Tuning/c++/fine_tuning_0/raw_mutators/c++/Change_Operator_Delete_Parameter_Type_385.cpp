//header file
#pragma once
#include "Mutator_base.h"

/**
 * Change_Operator_Delete_Parameter_Type_385
 */ 
class MutatorFrontendAction_385 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(385)

private:
    class MutatorASTConsumer_385 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_385(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<std::string> class_names;
    };
};

//source file
#include "../include/Change_Operator_Delete_Parameter_Type_385.h"

// ========================================================================================================
#define MUT385_OUTPUT 1

void MutatorFrontendAction_385::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CL->getLocation()))
        return;
      if ((!CL->isStruct() && !CL->isClass()) || CL->isLambda())
        return;
      if (!CL->isCompleteDefinition())
        return;
      class_names.push_back(CL->getNameAsString());
    } else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Funcs")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (!FD->isOverloadedOperator())
        return;
      if (FD->getOverloadedOperator() != OverloadedOperatorKind::OO_Delete)
        return;
      if (FD->getNumParams() != 1)
        return;
      auto param = FD->getParamDecl(0);
      auto param_type = param->getType();
      if (param_type->isPointerType())
        return;
      if (param_type->isBuiltinType())
        return;
      auto param_type_str = stringutils::rangetoStr(
          *(Result.SourceManager), param->getSourceRange());
      llvm::outs() << param_type_str << '\n';
      for (std::size_t i = 0; i < class_names.size(); ++i) {
        if (param_type_str == class_names[i])
          return;
      }
      string new_type = class_names[0];
      Rewrite.ReplaceText(param->getSourceRange(), new_type);
    }
  }
  
void MutatorFrontendAction_385::MutatorASTConsumer_385::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto class_matcher = cxxRecordDecl().bind("Classes");
    auto func_matcher = functionDecl().bind("Funcs");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(class_matcher, &callback);
    matchFinder.addMatcher(func_matcher, &callback);
    matchFinder.matchAST(Context);
}