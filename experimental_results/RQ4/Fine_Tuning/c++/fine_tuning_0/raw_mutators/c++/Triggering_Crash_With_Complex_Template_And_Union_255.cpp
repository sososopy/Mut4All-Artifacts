//header file
#pragma once
#include "Mutator_base.h"

/**
 * Triggering_Crash_With_Complex_Template_And_Union_255
 */ 
class MutatorFrontendAction_255 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(255)

private:
    class MutatorASTConsumer_255 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_255(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl *> cur_unions;
        std::vector<const clang::ClassTemplateDecl *> cur_templates;
    };
};

//source file
#include "../include/Triggering_Crash_With_Complex_Template_And_Union_255.h"

// ========================================================================================================
#define MUT255_OUTPUT 1

void MutatorFrontendAction_255::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Unions")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isUnion()) {
        cur_unions.push_back(DL);
      }
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>(
                   "Templates")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      cur_templates.push_back(DL);
    } else if (auto *DL =
                   Result.Nodes.getNodeAs<clang::TranslationUnitDecl>("TUs")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (cur_unions.empty()) {
        auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                               DL->getSourceRange());
        std::string new_union = "/*mut255*/union Mut255{int a;};\n";
        content = new_union + content;
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(DL->getSourceRange()), content);
      }
      if (cur_templates.empty()) {
        auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                               DL->getSourceRange());
        std::string new_template =
            "/*mut255*/template <typename T> class Mut255{T value; public: "
            "Mut255(T v) : value(v) {}};\n";
        content = new_template + content;
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(DL->getSourceRange()), content);
      }
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::TranslationUnitDecl>(
                   "TUsAfter")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (cur_unions.empty() || cur_templates.empty())
        return;
      size_t union_index = getrandom::getRandomIndex(cur_unions.size() - 1);
      size_t template_index = getrandom::getRandomIndex(cur_templates.size() - 1);
      auto target_union = cur_unions[union_index];
      auto target_template = cur_templates[template_index];
      auto union_content = stringutils::rangetoStr(
          *(Result.SourceManager), target_union->getSourceRange());
      auto template_content = stringutils::rangetoStr(
          *(Result.SourceManager), target_template->getSourceRange());
      auto template_name = target_template->getNameAsString();
      auto template_specialization = template_name + "<int>";
      auto new_union_content = union_content;
      if (union_content.rfind('}') != string::npos)
        new_union_content.insert(union_content.rfind('}'),
                                 template_specialization + " a;\n");
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(
                              target_union->getSourceRange()),
                          new_union_content);
    }
  }
  
void MutatorFrontendAction_255::MutatorASTConsumer_255::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher_union = cxxRecordDecl().bind("Unions");
    auto matcher_template = classTemplateDecl().bind("Templates");
    auto matcher_translationunit = translationUnitDecl().bind("TUs");
    auto matcher_translationunit_after = translationUnitDecl().bind("TUsAfter");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher_union, &callback);
    matchFinder.addMatcher(matcher_template, &callback);
    matchFinder.addMatcher(matcher_translationunit, &callback);
    matchFinder.matchAST(Context);
    MatchFinder matchFinder_after;
    matchFinder_after.addMatcher(matcher_translationunit_after, &callback);
    matchFinder_after.matchAST(Context);
}