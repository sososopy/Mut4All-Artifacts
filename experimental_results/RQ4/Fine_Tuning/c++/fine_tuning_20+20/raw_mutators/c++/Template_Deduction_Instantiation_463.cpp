//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Deduction_Instantiation_463
 */ 
class MutatorFrontendAction_463 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(463)

private:
    class MutatorASTConsumer_463 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_463(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
        std::vector<const clang::FunctionTemplateDecl *> target_templates;
    };
};

//source file
#include "../include/template_deduction_instantiation_463.h"

// ========================================================================================================
#define MUT463_OUTPUT 1

void MutatorFrontendAction_463::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("Templates")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FT->getLocation()))
        return;
      if (FT->getTemplateParameters()->size() < 1)
        return;
      target_templates.push_back(FT);
    } else if (auto *CL =
                   Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CL->getLocation()))
        return;
      if ((!CL->isStruct() && !CL->isClass()) || CL->isLambda())
        return;
      if (!CL->isCompleteDefinition())
        return;
      if (target_templates.empty())
        return;
      int index = getrandom::getRandomIndex(target_templates.size() - 1);
      auto target = target_templates[index];
      auto tp_list = target->getTemplateParameters();
      int tp_index = getrandom::getRandomIndex(tp_list->size() - 1);
      auto tp = tp_list->getParam(tp_index);
      auto tp_name = tp->getNameAsString();
      auto fd = target->getTemplatedDecl();
      auto params = fd->parameters();
      std::vector<int> target_params;
      for (size_t i = 0; i < params.size(); ++i) {
        if (params[i]->getType().getAsString().find(tp_name) != string::npos)
          target_params.push_back(i);
      }
      if (target_params.empty())
        return;
      int param_index = getrandom::getRandomIndex(target_params.size() - 1);
      auto param = params[target_params[param_index]];
      auto param_name = param->getNameAsString();
      auto param_type = param->getType().getAsString();
      string new_tp = "typename " + tp_name + "_ptr";
      string new_param_type = tp_name + "_ptr";
      if (param_type.find('&') != string::npos) {
        new_tp = tp_name + " *";
        new_param_type = tp_name + "_ptr";
      } else if (param_type.find('*') != string::npos) {
        new_tp = tp_name + " &";
        new_param_type = tp_name + "_ptr";
      } else {
        new_tp = tp_name + " *";
        new_param_type = tp_name + "_ptr";
      }
      llvm::outs() << new_tp << '\n';
      llvm::outs() << new_param_type << '\n';
      auto tp_str =
          stringutils::rangetoStr(*(Result.SourceManager), tp->getSourceRange());
      tp_str += ", " + new_tp;
      auto param_str = new_param_type + " " + param_name;
      llvm::outs() << tp_str << '\n';
      llvm::outs() << param_str << '\n';
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(tp->getSourceRange()),
                          tp_str);
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(param->getSourceRange()), param_str);
      auto class_name = CL->getNameAsString();
      auto call_expr = target->getNameAsString() + "<" + class_name + ">(" +
                       class_name + "())";
      Rewrite.ReplaceText(CL->getEndLoc(), 0, "/*mut463*/" + call_expr + ";\n");
    }
  }
  
void MutatorFrontendAction_463::MutatorASTConsumer_463::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionTemplateDecl().bind("Templates");
    auto class_matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(class_matcher, &callback);
    matchFinder.matchAST(Context);
}