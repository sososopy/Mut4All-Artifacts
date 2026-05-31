//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Ellipsis_And_Reorder_Template_Parameters_333
 */ 
class MutatorFrontendAction_333 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(333)

private:
    class MutatorASTConsumer_333 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_333(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::TemplateTypeParmDecl *> type_parms;
        std::vector<const clang::NonTypeTemplateParmDecl *> nontype_parms;
        std::vector<const clang::TemplateTemplateParmDecl *> template_parms;
    };
};

//source file
#include "../include/Replace_Ellipsis_And_Reorder_Template_Parameters_333.h"

// ========================================================================================================
#define MUT333_OUTPUT 1

void MutatorFrontendAction_333::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TL = Result.Nodes.getNodeAs<clang::TemplateParameterList>("TemplateLists")) {
      if (!TL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TL->getLAngleLoc()))
        return;
      auto params = TL->getParams();
      if (params.size() < 2)
        return;
      bool has_ellipsis = false;
      for (auto param : params) {
        if (param->isTemplateParameterPack()) {
          has_ellipsis = true;
          break;
        }
      }
      if (has_ellipsis == false)
        return;
      llvm::outs() << "Mut333: " << TL->size() << '\n';
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), TL->getSourceRange());
      if (content.find("...") == string::npos)
        return;
      for (auto param : params) {
        if (auto *TP = dyn_cast<TemplateTypeParmDecl>(param)) {
          type_parms.push_back(TP);
        } else if (auto *NP = dyn_cast<NonTypeTemplateParmDecl>(param)) {
          nontype_parms.push_back(NP);
        } else if (auto *TPP = dyn_cast<TemplateTemplateParmDecl>(param)) {
          template_parms.push_back(TPP);
        }
      }
      if (type_parms.size() + nontype_parms.size() + template_parms.size() !=
          params.size())
        return;
      if (type_parms.empty())
        return;
      int index = getrandom::getRandomIndex(type_parms.size() - 1);
      auto target = type_parms[index];
      auto target_text = stringutils::rangetoStr(*(Result.SourceManager),
                                                 target->getSourceRange());
      llvm::outs() << target_text << '\n';
      content.erase(content.find(target_text), target_text.size());
      content = target_text + "...," + content;
      content = "/*mut333*/" + content;
      Rewrite.ReplaceText(TL->getSourceRange(), content);
    }
}
  
void MutatorFrontendAction_333::MutatorASTConsumer_333::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = templateParameterList().bind("TemplateLists");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}