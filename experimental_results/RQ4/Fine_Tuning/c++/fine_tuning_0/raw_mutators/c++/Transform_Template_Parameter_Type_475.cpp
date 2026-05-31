//header file
#pragma once
#include "Mutator_base.h"

/**
 * Transform_Template_Parameter_Type_475
 */ 
class MutatorFrontendAction_475 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(475)

private:
    class MutatorASTConsumer_475 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_475(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::TemplateDecl *> templates;
        std::vector<const clang::TemplateSpecializationType *> template_specializations;
    };
};

//source file
#include "../include/Transform_Template_Parameter_Type_475.h"

// ========================================================================================================
#define MUT475_OUTPUT 1

void MutatorFrontendAction_475::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::TemplateDecl>("Templates")) {
    if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                   TD->getBeginLoc()))
      return;
    templates.push_back(TD);
  } else if (auto *TS = Result.Nodes.getNodeAs<clang::TemplateSpecializationType>(
                 "TemplateSpecializations")) {
    if (!TS || !Result.Context->getSourceManager().isWrittenInMainFile(
                   TS->getBeginLoc()))
      return;
    template_specializations.push_back(TS);
  }
  if (templates.size() == 0)
    return;
  int index = getrandom::getRandomIndex(templates.size() - 1);
  auto target_template = templates[index];
  auto template_name = target_template->getNameAsString();
  auto template_parameters = target_template->getTemplateParameters();
  if (template_parameters->size() == 0)
    return;
  int param_index = getrandom::getRandomIndex(template_parameters->size() - 1);
  auto target_param = template_parameters->getParam(param_index);
  auto target_param_name = target_param->getNameAsString();
  auto target_param_type = target_param->getKind();
  llvm::outs() << "target template: " << template_name << '\n';
  llvm::outs() << "target param: " << target_param_name << '\n';
  llvm::outs() << "target param type: " << target_param_type << '\n';
  if (target_param_type == TemplateParameter::ParameterKind::NonTypeTemplate) {
    auto NTTP = dyn_cast<clang::NonTypeTemplateParmDecl>(target_param);
    auto NTTP_type = NTTP->getType();
    llvm::outs() << "NTTP type: " << NTTP_type.getAsString() << '\n';
    if (NTTP_type->isIntegerType()) {
      auto NTTP_type_name = NTTP_type.getAsString();
      string new_type_name = "long";
      if (NTTP_type_name == "long")
        new_type_name = "unsigned long";
      llvm::outs() << "new type name: " << new_type_name << '\n';
      for (auto template_specialization : template_specializations) {
        auto template_name_info = template_specialization->getTemplateName();
        auto template_name = template_name_info.getAsTemplateDecl()->getName();
        if (template_name != target_template->getName())
          continue;
        auto args = template_specialization->template_arguments();
        for (auto arg : args) {
          if (arg.getKind() == TemplateArgument::ArgKind::Expression) {
            auto arg_expr = arg.getAsExpr();
            auto arg_expr_type = arg_expr->getType();
            if (arg_expr_type == NTTP_type) {
              auto arg_expr_str = stringutils::rangetoStr(
                  *(Result.SourceManager), arg_expr->getSourceRange());
              llvm::outs() << "arg expr str: " << arg_expr_str << '\n';
              Rewrite.ReplaceText(arg_expr->getBeginLoc(), 0,
                                  "(" + new_type_name + ")");
            }
          }
        }
      }
      Rewrite.ReplaceText(NTTP->getTypeSpecStartLoc(), NTTP_type_name.length(),
                          new_type_name);
    }
  } else if (target_param_type ==
             TemplateParameter::ParameterKind::TypeTemplate) {
    auto TTP = dyn_cast<clang::TemplateTypeParmDecl>(target_param);
    auto TTP_name = TTP->getNameAsString();
    llvm::outs() << "TTP name: " << TTP_name << '\n';
    if (TTP_name == "")
      return;
    for (auto template_specialization : template_specializations) {
      auto template_name_info = template_specialization->getTemplateName();
      auto template_name = template_name_info.getAsTemplateDecl()->getName();
      if (template_name != target_template->getName())
        continue;
      auto args = template_specialization->template_arguments();
      for (auto arg : args) {
        if (arg.getKind() == TemplateArgument::ArgKind::Type) {
          auto arg_type = arg.getAsType();
          auto arg_type_name = arg_type.getAsString();
          llvm::outs() << "arg type name: " << arg_type_name << '\n';
        }
      }
    }
    if (index == 0 && templates.size() > 1)
      index = 1;
    else
      index = 0;
    auto new_template = templates[index];
    auto new_template_name = new_template->getNameAsString();
    llvm::outs() << "new template name: " << new_template_name << '\n';
    Rewrite.ReplaceText(TTP->getLocation(), TTP_name.length(),
                        new_template_name);
    for (auto template_specialization : template_specializations) {
      auto template_name_info = template_specialization->getTemplateName();
      auto template_name = template_name_info.getAsTemplateDecl()->getName();
      if (template_name != target_template->getName())
        continue;
      auto args = template_specialization->template_arguments();
      for (auto arg : args) {
        if (arg.getKind() == TemplateArgument::ArgKind::Type) {
          auto arg_type = arg.getAsType();
          auto arg_type_name = arg_type.getAsString();
          llvm::outs() << "arg type name: " << arg_type_name << '\n';
          Rewrite.ReplaceText(arg_type->getBeginLoc(), arg_type_name.length(),
                              new_template_name);
        }
      }
    }
  }
}

void MutatorFrontendAction_475::MutatorASTConsumer_475::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto template_matcher = templateDecl().bind("Templates");
  auto template_specialization_matcher =
      templateSpecializationType().bind("TemplateSpecializations");
  Callback callback(TheRewriter);
  matchFinder.addMatcher(template_matcher, &callback);
  matchFinder.addMatcher(template_specialization_matcher, &callback);
  matchFinder.matchAST(Context);
}