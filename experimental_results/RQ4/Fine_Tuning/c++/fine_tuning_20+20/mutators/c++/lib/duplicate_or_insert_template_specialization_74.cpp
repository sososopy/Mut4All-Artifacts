//source file
#include "../include/Duplicate_Or_Insert_Template_Specialization_74.h"

// ========================================================================================================
#define MUT74_OUTPUT 1

void MutatorFrontendAction_74::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TS = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("ClassSpecializations")) {
      if (!TS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TS->getBeginLoc()))
        return;
      cur_class_specializations.push_back(TS);
      if (TS->isExplicitSpecialization())
        cur_class_specialization_type.push_back(0);
      else
        cur_class_specialization_type.push_back(1);
    } else if (auto *FS = Result.Nodes.getNodeAs<clang::FunctionDecl>(
                   "FunctionSpecializations")) {
      if (!FS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FS->getBeginLoc()))
        return;
      if (FS->getTemplatedKind() !=
          FunctionDecl::TemplatedKind::TK_FunctionTemplateSpecialization)
        return;
      cur_function_specializations.push_back(FS);
      if (FS->isFunctionTemplateSpecialization())
        cur_function_specialization_type.push_back(0);
      else
        cur_function_specialization_type.push_back(1);
    } else if (auto *CT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>(
                   "ClassTemplates")) {
      if (!CT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CT->getBeginLoc()))
        return;
      cur_class_templates.push_back(CT);
    } else if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>(
                   "FunctionTemplates")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FT->getBeginLoc()))
        return;
      cur_function_templates.push_back(FT);
    }
  }

void MutatorFrontendAction_74::MutatorASTConsumer_74::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto class_specialization_matcher =
        classTemplateSpecializationDecl().bind("ClassSpecializations");
    auto function_specialization_matcher =
        functionDecl().bind("FunctionSpecializations");
    auto class_template_matcher = classTemplateDecl().bind("ClassTemplates");
    auto function_template_matcher =
        functionTemplateDecl().bind("FunctionTemplates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(class_specialization_matcher, &callback);
    matchFinder.addMatcher(function_specialization_matcher, &callback);
    matchFinder.addMatcher(class_template_matcher, &callback);
    matchFinder.addMatcher(function_template_matcher, &callback);
    matchFinder.matchAST(Context);
    if (!callback.cur_class_specializations.empty()) {
      size_t index = getrandom::getRandomIndex(callback.cur_class_specializations.size() - 1);
      auto target = callback.cur_class_specializations[index];
      auto content = stringutils::rangetoStr((Context.getSourceManager()),
                                             target->getSourceRange());
      if (callback.cur_class_specialization_type[index] == 0) {
        content = "/*mut74*/" + content;
        TheRewriter.ReplaceText(target->getEndLoc(), 0, content);
      } else if (callback.cur_class_specialization_type[index] == 1) {
        content = "/*mut74*/template<>\n" + content;
        TheRewriter.ReplaceText(target->getEndLoc(), 0, content);
      }
    } else if (!callback.cur_function_specializations.empty()) {
      size_t index =
          getrandom::getRandomIndex(callback.cur_function_specializations.size() - 1);
      auto target = callback.cur_function_specializations[index];
      auto content = stringutils::rangetoStr((Context.getSourceManager()),
                                             target->getSourceRange());
      if (callback.cur_function_specialization_type[index] == 0) {
        content = "/*mut74*/" + content;
        TheRewriter.ReplaceText(target->getEndLoc(), 0, content);
      } else if (callback.cur_function_specialization_type[index] == 1) {
        content = "/*mut74*/template<>\n" + content;
        TheRewriter.ReplaceText(target->getEndLoc(), 0, content);
      }
    } else if (!callback.cur_class_templates.empty()) {
      size_t index = getrandom::getRandomIndex(callback.cur_class_templates.size() - 1);
      auto target = callback.cur_class_templates[index];
      auto content = stringutils::rangetoStr((Context.getSourceManager()),
                                             target->getSourceRange());
      content = "/*mut74*/template<>\n" + content;
      TheRewriter.ReplaceText(target->getEndLoc(), 0, content);
    } else if (!callback.cur_function_templates.empty()) {
      size_t index = getrandom::getRandomIndex(callback.cur_function_templates.size() - 1);
      auto target = callback.cur_function_templates[index];
      auto content = stringutils::rangetoStr((Context.getSourceManager()),
                                             target->getSourceRange());
      content = "/*mut74*/template<>\n" + content;
      TheRewriter.ReplaceText(target->getEndLoc(), 0, content);
    }
}