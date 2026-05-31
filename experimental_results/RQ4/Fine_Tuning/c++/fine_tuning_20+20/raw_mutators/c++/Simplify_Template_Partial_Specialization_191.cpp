//header file
#pragma once
#include "Mutator_base.h"

/**
 * Simplify_Template_Partial_Specialization_191
 */ 
class MutatorFrontendAction_191 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(191)

private:
    class MutatorASTConsumer_191 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_191(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::ClassTemplateDecl *> cur_templates;
    };
};

//source file
#include "../include/Simplify_Template_Partial_Specialization_191.h"

// ========================================================================================================
#define MUT191_OUTPUT 1

void MutatorFrontendAction_191::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TP = Result.Nodes.getNodeAs<clang::ClassTemplatePartialSpecializationDecl>("PartialSpecializations")) {
      if (!TP || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TP->getLocation()))
        return;
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), TP->getSourceRange());
      llvm::outs() << content << '\n';
      auto tpl = TP->getSpecializedTemplate();
      if (!tpl)
        return;
      if (TP->getTemplateParameters()->size() > 1) {
        auto pos = content.find('<');
        if (pos != string::npos) {
          auto rpos = content.rfind('>');
          if (rpos != string::npos) {
            content.erase(pos, rpos - pos + 1);
          }
        }
      } else {
        auto pos = content.find('<');
        if (pos != string::npos) {
          auto rpos = content.rfind('>');
          if (rpos != string::npos) {
            content.erase(pos, rpos - pos + 1);
            content.insert(pos, "<int>");
          }
        }
      }
      content = "/*mut191*/" + content;
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(TP->getSourceRange()), content);
    } else if (auto *TP = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>(
                   "Templates")) {
      if (!TP || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TP->getLocation()))
        return;
      cur_templates.push_back(TP);
    }
}

void MutatorFrontendAction_191::MutatorASTConsumer_191::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto partialspecialization_matcher =
        classTemplatePartialSpecializationDecl().bind("PartialSpecializations");
    auto template_matcher = classTemplateDecl().bind("Templates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(partialspecialization_matcher, &callback);
    matchFinder.addMatcher(template_matcher, &callback);
    matchFinder.matchAST(Context);
}