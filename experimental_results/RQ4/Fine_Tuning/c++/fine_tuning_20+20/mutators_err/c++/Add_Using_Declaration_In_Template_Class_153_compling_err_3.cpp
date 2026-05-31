//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Using_Declaration_In_Template_Class_153
 */ 
class MutatorFrontendAction_153 : public MutatorFrontendAction {
    public:
      MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(153)
    
    private:
      class MutatorASTConsumer_153 : public MutatorASTConsumer {
        public:
          MutatorASTConsumer_153(Rewriter &R) : TheRewriter(R) {}
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
          std::vector<const clang::CXXRecordDecl *> cur_classes;
      };
    };

//source file
#include "../include/add_using_declaration_in_template_class_153"

// ========================================================================================================
#define MUT153_OUTPUT 1

void MutatorFrontendAction_153::Callback::run(const MatchFinder::MatchResult &Result) {
  if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isCompleteDefinition() == false)
      return;
    if (DL->isTemplated() == false)
      return;
    cur_classes.push_back(DL);
  } else if (auto *TL = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>(
                 "Templates")) {
    if (!TL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   TL->getLocation()))
      return;
    auto DL = TL->getTemplatedDecl();
    if (DL->isCompleteDefinition() == false)
      return;
    if (DL->isTemplated() == false)
      return;
    if (DL->hasDefinition() == false)
      return;
    auto bases = DL->bases();
    if (bases.empty())
      return;
    auto base = bases.begin();
    auto basetype = base->getType();
    if (basetype.getTypePtr()->isDependentType() == false)
      return;
    auto baserecord = basetype.getTypePtr()->getAsCXXRecordDecl();
    if (baserecord == nullptr)
      return;
    if (baserecord->isCompleteDefinition() == false)
      return;
    auto decls = baserecord->decls();
    std::vector<string> target_names;
    for (auto decl : decls) {
      if (decl->getIdentifierNamespace() ==
          Decl::IdentifierNamespace::IDNS_Ordinary) {
        auto name = decl->getNameAsString();
        target_names.push_back(name);
      }
    }
    if (target_names.empty())
      return;
    auto target_index = getrandom::getRandomIndex(target_names.size() - 1);
    auto target_name = target_names[target_index];
    auto base_name = baserecord->getNameAsString();
    auto template_params = TL->getTemplateParameters();
    auto template_num = template_params->size();
    auto template_index = getrandom::getRandomIndex(template_num - 1);
    auto template_param = template_params->getParam(template_index);
    auto template_param_name = template_param->getNameAsString();
    auto using_decl = "\n/*mut153*/using typename " + base_name + "<" +
                      template_param_name + ">::" + target_name + ";\n";
    Rewrite.ReplaceText(DL->getEndLoc(), 0, using_decl);
  } else if (auto *NL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>(
                 "NonTemplates")) {
    if (!NL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   NL->getLocation()))
      return;
    if (NL->isCompleteDefinition() == false)
      return;
    if (NL->isTemplated() == true)
      return;
    if (cur_classes.empty())
      return;
    auto target_index = getrandom::getRandomIndex(cur_classes.size() - 1);
    auto target_class = cur_classes[target_index];
    auto template_name = target_class->getNameAsString();
    auto template_params = target_class->getTemplateArgsAsString();
    auto class_name = NL->getNameAsString();
    auto new_class_name = "/*mut153*/template<" + template_params +
                          ">\nstruct " + class_name + " : " + template_name +
                          "<" + template_params + "> {}";
    Rewrite.ReplaceText(NL->getSourceRange(), new_class_name);
  }
}

void MutatorFrontendAction_153::MutatorASTConsumer_153::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Classes");
    auto template_matcher = classTemplateDecl().bind("Templates");
    auto nontemplate_matcher = cxxRecordDecl().bind("NonTemplates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(template_matcher, &callback);
    matchFinder.addMatcher(nontemplate_matcher, &callback);
    matchFinder.matchAST(Context);
}