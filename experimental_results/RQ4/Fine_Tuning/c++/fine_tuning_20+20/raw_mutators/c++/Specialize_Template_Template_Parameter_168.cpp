//header file
#pragma once
#include "Mutator_base.h"

/**
 * Specialize_Template_Template_Parameter_168
 */ 
class MutatorFrontendAction_168 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(168)

private:
    class MutatorASTConsumer_168 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_168(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<string> nested_template_name;
        std::vector<string> nested_template_content;
        std::vector<string> partial_specialization_name;
        std::vector<string> partial_specialization_content;
    };
};

//source file
#include "../include/Specialize_Template_Template_Parameter_168.h"

// ========================================================================================================
#define MUT168_OUTPUT 1

void MutatorFrontendAction_168::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Templates")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (!DL->isThisDeclarationADefinition())
        return;
      if (DL->getTemplateParameters()->size() == 0)
        return;
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      llvm::outs() << content << '\n';
      auto params = DL->getTemplateParameters();
      for (auto param : *params) {
        if (auto ttp = dyn_cast<TemplateTemplateParmDecl>(param)) {
          llvm::outs() << "find template template param\n";
          auto ttp_content = stringutils::rangetoStr(
              *(Result.SourceManager), ttp->getSourceRange());
          llvm::outs() << ttp_content << '\n';
          auto ttp_name = ttp->getNameAsString();
          llvm::outs() << ttp_name << '\n';
          for (std::size_t i = 0; i < nested_template_name.size(); ++i) {
            llvm::outs() << nested_template_name[i] << '\n';
            if (nested_template_name[i] == ttp_name) {
              content = stringutils::str_replace(
                  content, ttp_content, nested_template_content[i]);
              llvm::outs() << "replace with nested template\n";
              break;
            }
          }
          for (std::size_t i = 0; i < partial_specialization_name.size(); ++i) {
            llvm::outs() << partial_specialization_name[i] << '\n';
            if (partial_specialization_name[i] == ttp_name) {
              content = stringutils::str_replace(
                  content, ttp_content, partial_specialization_content[i]);
              llvm::outs() << "replace with partial specialization\n";
              break;
            }
          }
        }
      }
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(DL->getSourceRange()), content);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>(
                   "NestedTemplates")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (!DL->isThisDeclarationADefinition())
        return;
      if (DL->getTemplateParameters()->size() == 0)
        return;
      auto params = DL->getTemplateParameters();
      for (auto param : *params) {
        if (auto ttp = dyn_cast<TemplateTemplateParmDecl>(param)) {
          auto ttp_name = ttp->getNameAsString();
          auto nested_content =
              stringutils::rangetoStr(*(Result.SourceManager),
                                      DL->getSourceRange());
          nested_template_name.push_back(ttp_name);
          nested_template_content.push_back(nested_content);
        }
      }
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::ClassTemplatePartialSpecializationDecl>(
                   "PartialSpecializations")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      auto params = DL->getTemplateParameters();
      for (auto param : *params) {
        if (auto ttp = dyn_cast<TemplateTemplateParmDecl>(param)) {
          auto ttp_name = ttp->getNameAsString();
          auto partial_content =
              stringutils::rangetoStr(*(Result.SourceManager),
                                      DL->getSourceRange());
          partial_specialization_name.push_back(ttp_name);
          partial_specialization_content.push_back(partial_content);
        }
      }
    }
}
  
void MutatorFrontendAction_168::MutatorASTConsumer_168::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = classTemplateDecl().bind("Templates");
    auto nested_matcher = classTemplateDecl(hasDescendant(classTemplateDecl())).bind("NestedTemplates");
    auto partial_matcher = classTemplatePartialSpecializationDecl().bind("PartialSpecializations");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(nested_matcher, &callback);
    matchFinder.addMatcher(partial_matcher, &callback);
    matchFinder.matchAST(Context);
}