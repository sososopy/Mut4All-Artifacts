//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_CTAD_For_Alias_Template_With_Nested_Template_Classes_53
 */ 
class MutatorFrontendAction_53 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(53)

private:
    class MutatorASTConsumer_53 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_53(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
        const clang::FunctionDecl *MainFunc;
        std::vector<const clang::TypeAliasTemplateDecl *> AliasTemplates;
        std::vector<const clang::ClassTemplateDecl *> ClassTemplates;
    private:
        Rewriter &Rewrite;
    };
};

//source file
#include "../include/Add_CTAD_For_Alias_Template_With_Nested_Template_Classes_53.h"

// ========================================================================================================
#define MUT53_OUTPUT 1

void MutatorFrontendAction_53::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("MainFunc")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getNameAsString() == "main")
        MainFunc = FD;
    } else if (auto *AT =
                   Result.Nodes.getNodeAs<clang::TypeAliasTemplateDecl>(
                       "AliasTemplate")) {
      if (!AT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     AT->getLocation()))
        return;
      AliasTemplates.push_back(AT);
    } else if (auto *CT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>(
                   "ClassTemplate")) {
      if (!CT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CT->getLocation()))
        return;
      ClassTemplates.push_back(CT);
    }
  }

void MutatorFrontendAction_53::MutatorASTConsumer_53::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto mainfunc_matcher = functionDecl().bind("MainFunc");
    auto alias_template_matcher = typeAliasTemplateDecl().bind("AliasTemplate");
    auto class_template_matcher = classTemplateDecl().bind("ClassTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(mainfunc_matcher, &callback);
    matchFinder.addMatcher(alias_template_matcher, &callback);
    matchFinder.addMatcher(class_template_matcher, &callback);
    matchFinder.matchAST(Context);
    if (callback.MainFunc == nullptr) {
      llvm::errs() << "Mut53: No main function found!\n";
      return;
    }
    if (callback.AliasTemplates.empty() || callback.ClassTemplates.empty()) {
      llvm::errs()
          << "Mut53: No alias template or no template class found!\n";
      return;
    }
    if (callback.AliasTemplates.size() == 1 &&
        callback.ClassTemplates.size() == 1) {
      auto alias_template = callback.AliasTemplates[0];
      auto class_template = callback.ClassTemplates[0];
      auto alias_type =
          alias_template->getTemplatedDecl()->getUnderlyingType();
      if (alias_type->isTemplateSpecializationType()) {
        auto ts = alias_type->getAs<TemplateSpecializationType>();
        auto aliased_template = ts->getTemplateName().getAsTemplateDecl();
        if (aliased_template == class_template) {
          auto alias_name = alias_template->getNameAsString();
          auto insert_code = "/*mut53*/" + alias_name + " test{42};\n";
          TheRewriter.ReplaceText(callback.MainFunc->getBody()->getBeginLoc(), 0,
                              insert_code);
        } else {
          llvm::errs() << "Mut53: The alias template does not use the "
                          "template class!\n";
        }
      } else {
        llvm::errs() << "Mut53: The alias template does not use the "
                        "template class!\n";
      }
    } else {
      std::vector<int> candidate_alias;
      std::vector<int> candidate_class;
      for (std::size_t i = 0; i < callback.AliasTemplates.size(); ++i) {
        auto alias_template = callback.AliasTemplates[i];
        auto alias_type =
            alias_template->getTemplatedDecl()->getUnderlyingType();
        if (alias_type->isTemplateSpecializationType()) {
          auto ts = alias_type->getAs<TemplateSpecializationType>();
          auto aliased_template = ts->getTemplateName().getAsTemplateDecl();
          for (std::size_t j = 0; j < callback.ClassTemplates.size(); ++j) {
            auto class_template = callback.ClassTemplates[j];
            if (aliased_template == class_template) {
              candidate_alias.push_back(i);
              candidate_class.push_back(j);
            }
          }
        }
      }
      if (candidate_alias.empty()) {
        llvm::errs() << "Mut53: No alias template uses any template class!\n";
        return;
      }
      auto alias_template = callback.AliasTemplates[candidate_alias[0]];
      auto alias_name = alias_template->getNameAsString();
      auto insert_code = "/*mut53*/" + alias_name + " test{42};\n";
      TheRewriter.ReplaceText(callback.MainFunc->getBody()->getBeginLoc(), 0,
                          insert_code);
    }
}