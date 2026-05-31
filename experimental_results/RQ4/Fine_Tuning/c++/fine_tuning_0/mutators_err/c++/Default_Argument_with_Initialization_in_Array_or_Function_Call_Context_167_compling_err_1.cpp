//header file
#pragma once
#include "Mutator_base.h"

/**
 * Default_Argument_with_Initialization_in_Array_or_Function_Call_Context_167
 */ 
class MutatorFrontendAction_167 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(167)

private:
    class MutatorASTConsumer_167 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_167(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl *> cur_classes;
    };
};

//source file
#include "../include/Default_Argument_with_Initialization_in_Array_or_Function_Call_Context_167.h"

// ========================================================================================================
#define MUT167_OUTPUT 1

void MutatorFrontendAction_167::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isCompleteDefinition() == false)
      return;
    cur_classes.push_back(DL);
  } else if (auto *DL =
                 Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isCompleteDefinition() == false)
      return;
    cur_functions.push_back(DL);
  } else if (auto *DL =
                 Result.Nodes.getNodeAs<clang::InitListExpr>("InitLists")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getBeginLoc()))
      return;
    if (DL->getNumInits() != 0)
      return;
    for (auto cur_class : cur_classes) {
      if (DL->isConstantInitializer(*Result.Context, false) &&
          DL->isConstantInitializer(*Result.Context, true)) {
        if (DL->isConstantInitializer(*Result.Context, false) &&
            DL->isConstantInitializer(*Result.Context, true)) {
          auto constructors = cur_class->ctors();
          for (auto ctor : constructors) {
            if (ctor->getNumParams() == 1 &&
                ctor->getParamDecl(0)->hasDefaultArg()) {
              auto class_name = cur_class->getNameAsString();
              auto ins = class_name + "()";
              std::string ins_list = "{";
              for (int i = 0; i < DL->getArrayFiller()->EvaluateKnownConstInt(
                                      *Result.Context)
                                      .getExtValue();
                   ++i) {
                ins_list += ins;
                if (i != DL->getArrayFiller()
                             ->EvaluateKnownConstInt(*Result.Context)
                             .getExtValue() -
                         1)
                  ins_list += ",";
              }
              ins_list += "}";
              llvm::outs() << ins_list;
              Rewrite.ReplaceText(
                  CharSourceRange::getTokenRange(DL->getSourceRange()),
                  "/*mut167*/" + ins_list);
            }
          }
        }
      }
    }
  } else if (auto *DL =
                 Result.Nodes.getNodeAs<clang::CallExpr>("CallFunctions")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getBeginLoc()))
      return;
    if (DL->getNumArgs() != 0)
      return;
    for (auto cur_function : cur_functions) {
      if (DL->isConstantInitializer(*Result.Context, false) &&
          DL->isConstantInitializer(*Result.Context, true)) {
        if (DL->isConstantInitializer(*Result.Context, false) &&
            DL->isConstantInitializer(*Result.Context, true)) {
          if (cur_function->getNumParams() == 1 &&
              cur_function->getParamDecl(0)->hasDefaultArg()) {
            auto default_expr = cur_function->getParamDecl(0)->getDefaultArg();
            auto default_expr_str = stringutils::rangetoStr(
                *(Result.SourceManager), default_expr->getSourceRange());
            llvm::outs() << default_expr_str;
            Rewrite.ReplaceText(
                CharSourceRange::getTokenRange(DL->getSourceRange()),
                "/*mut167*/" + default_expr_str);
          }
        }
      }
    }
  }
}
  
void MutatorFrontendAction_167::MutatorASTConsumer_167::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Classes");
    auto funtion_matcher = functionDecl().bind("Functions");
    auto initlist_matcher = initListExpr().bind("InitLists");
    auto call_expr_matcher = callExpr().bind("CallFunctions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(funtion_matcher, &callback);
    matchFinder.addMatcher(initlist_matcher, &callback);
    matchFinder.addMatcher(call_expr_matcher, &callback);
    matchFinder.matchAST(Context);
}