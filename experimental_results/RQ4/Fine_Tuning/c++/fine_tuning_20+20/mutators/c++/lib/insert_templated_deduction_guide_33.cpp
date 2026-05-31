//source file
#include "../include/insert_templated_deduction_guide_33.h"

// ========================================================================================================
#define MUT33_OUTPUT 1

void MutatorFrontendAction_33::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Templates")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      // llvm::outs() << DL->getNameAsString() << '\n';
      cur_templates.push_back(DL);
    } else if (auto *DG = Result.Nodes.getNodeAs<clang::CXXDeductionGuideDecl>(
                   "DeductionGuides")) {
      if (!DG || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DG->getLocation()))
        return;
      if (DG->isImplicit())
        return;
      // llvm::outs() << DG->getNameAsString() << '\n';
      for (size_t i = 0; i < cur_templates.size(); ++i) {
        if (DG->getNameAsString() == cur_templates[i]->getNameAsString()) {
          cur_templates.erase(cur_templates.begin() + i);
          break;
        }
      }
    }
    // llvm::outs() << cur_templates.size() << '\n';
    else if (auto *DL = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>(
                 "FinalTemplates")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      bool flag = false;
      for (size_t i = 0; i < cur_templates.size(); ++i) {
        if (DL->getNameAsString() == cur_templates[i]->getNameAsString()) {
          flag = true;
          break;
        }
      }
      if (flag == false)
        return;
      auto params = DL->getTemplateParameters();
      string param_str = "template<";
      for (size_t i = 0; i < params->size(); ++i) {
        auto param = params->getParam(i);
        if (param->getKind() == Decl::Kind::TemplateTypeParm) {
          auto type_param = llvm::dyn_cast<clang::TemplateTypeParmDecl>(param);
          if (type_param->wasDeclaredWithTypename())
            param_str += "typename ";
          else
            param_str += "class ";
          param_str += type_param->getNameAsString();
        } else if (param->getKind() == Decl::Kind::NonTypeTemplateParm) {
          auto nontype_param =
              llvm::dyn_cast<clang::NonTypeTemplateParmDecl>(param);
          param_str += nontype_param->getType().getAsString();
          param_str += ' ';
          param_str += nontype_param->getNameAsString();
        } else if (param->getKind() == Decl::Kind::TemplateTemplateParm) {
          auto template_param =
              llvm::dyn_cast<clang::TemplateTemplateParmDecl>(param);
          param_str += "template<";
          auto sub_params = template_param->getTemplateParameters();
          for (size_t j = 0; j < sub_params->size(); ++j) {
            auto sub_param = sub_params->getParam(j);
            if (sub_param->getKind() == Decl::Kind::TemplateTypeParm) {
              auto type_param =
                  llvm::dyn_cast<clang::TemplateTypeParmDecl>(sub_param);
              if (type_param->wasDeclaredWithTypename())
                param_str += "typename ";
              else
                param_str += "class ";
              param_str += type_param->getNameAsString();
            } else if (sub_param->getKind() == Decl::Kind::NonTypeTemplateParm) {
              auto nontype_param =
                  llvm::dyn_cast<clang::NonTypeTemplateParmDecl>(sub_param);
              param_str += nontype_param->getType().getAsString();
              param_str += ' ';
              param_str += nontype_param->getNameAsString();
            } else if (sub_param->getKind() ==
                       Decl::Kind::TemplateTemplateParm) {
              auto template_param =
                  llvm::dyn_cast<clang::TemplateTemplateParmDecl>(sub_param);
              param_str += "template<";
              auto sub_params = template_param->getTemplateParameters();
              for (size_t k = 0; k < sub_params->size(); ++k) {
                auto sub_param = sub_params->getParam(k);
                if (sub_param->getKind() == Decl::Kind::TemplateTypeParm) {
                  auto type_param =
                      llvm::dyn_cast<clang::TemplateTypeParmDecl>(sub_param);
                  if (type_param->wasDeclaredWithTypename())
                    param_str += "typename ";
                  else
                    param_str += "class ";
                  param_str += type_param->getNameAsString();
                } else if (sub_param->getKind() ==
                           Decl::Kind::NonTypeTemplateParm) {
                  auto nontype_param =
                      llvm::dyn_cast<clang::NonTypeTemplateParmDecl>(sub_param);
                  param_str += nontype_param->getType().getAsString();
                  param_str += ' ';
                  param_str += nontype_param->getNameAsString();
                } else if (sub_param->getKind() ==
                           Decl::Kind::TemplateTemplateParm) {
                  auto template_param = llvm::dyn_cast<
                      clang::TemplateTemplateParmDecl>(sub_param);
                  param_str += "template<";
                  auto sub_params = template_param->getTemplateParameters();
                  for (size_t l = 0; l < sub_params->size(); ++l) {
                    auto sub_param = sub_params->getParam(l);
                    if (sub_param->getKind() == Decl::Kind::TemplateTypeParm) {
                      auto type_param = llvm::dyn_cast<
                          clang::TemplateTypeParmDecl>(sub_param);
                      if (type_param->wasDeclaredWithTypename())
                        param_str += "typename ";
                      else
                        param_str += "class ";
                      param_str += type_param->getNameAsString();
                    } else if (sub_param->getKind() ==
                               Decl::Kind::NonTypeTemplateParm) {
                      auto nontype_param = llvm::dyn_cast<
                          clang::NonTypeTemplateParmDecl>(sub_param);
                      param_str += nontype_param->getType().getAsString();
                      param_str += ' ';
                      param_str += nontype_param->getNameAsString();
                    }
                    if (l + 1 < sub_params->size())
                      param_str += ',';
                  }
                  param_str += "> ";
                  param_str += template_param->getNameAsString();
                }
                if (k + 1 < sub_params->size())
                  param_str += ',';
              }
              param_str += "> ";
              param_str += template_param->getNameAsString();
            }
            if (j + 1 < sub_params->size())
              param_str += ',';
          }
          param_str += "> ";
          param_str += template_param->getNameAsString();
        }
        if (i + 1 < params->size())
          param_str += ',';
      }
      param_str += ">\n";
      param_str += DL->getNameAsString();
      param_str += '(';
      for (size_t i = 0; i < params->size(); ++i) {
        auto param = params->getParam(i);
        if (param->getKind() == Decl::Kind::TemplateTypeParm) {
          auto type_param = llvm::dyn_cast<clang::TemplateTypeParmDecl>(param);
          param_str += type_param->getNameAsString();
        } else if (param->getKind() == Decl::Kind::NonTypeTemplateParm) {
          auto nontype_param =
              llvm::dyn_cast<clang::NonTypeTemplateParmDecl>(param);
          param_str += nontype_param->getNameAsString();
        } else if (param->getKind() == Decl::Kind::TemplateTemplateParm) {
          auto template_param =
              llvm::dyn_cast<clang::TemplateTemplateParmDecl>(param);
          param_str += template_param->getNameAsString();
        }
        if (i + 1 < params->size())
          param_str += ',';
      }
      param_str += ')';
      param_str += "{}\n";
      llvm::outs() << param_str;
      Rewrite.ReplaceText(DL->getEndLoc(), 0, "/*mut33*/" + param_str);
    }
}

void MutatorFrontendAction_33::MutatorASTConsumer_33::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = classTemplateDecl().bind("Templates");
    auto deductionguide_matcher = cxxDeductionGuideDecl().bind("DeductionGuides");
    auto final_matcher = classTemplateDecl().bind("FinalTemplates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(deductionguide_matcher, &callback);
    matchFinder.addMatcher(final_matcher, &callback);
    matchFinder.matchAST(Context);
}