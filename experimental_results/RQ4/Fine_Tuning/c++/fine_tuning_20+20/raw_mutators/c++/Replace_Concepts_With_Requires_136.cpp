//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Concepts_With_Requires_136
 */ 
class MutatorFrontendAction_136 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(136)

private:
    class MutatorASTConsumer_136 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_136(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::TemplateDecl *> cur_templates;
    };
};

//source file
#include "../include/Replace_Concepts_With_Requires_136.h"

// ========================================================================================================
#define MUT136_OUTPUT 1

void MutatorFrontendAction_136::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TP = Result.Nodes.getNodeAs<clang::TemplateDecl>("Templates")) {
      if (!TP || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TP->getLocation()))
        return;
      cur_templates.push_back(TP);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::DeclRefExpr>(
                   "Concepts")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      auto DLtype = DL->getType();
      if (DLtype->isDependentType() == false)
        return;
      auto DLname = DL->getNameInfo().getAsString();
      llvm::outs() << DLname << '\n';
      auto DLtemplate = DL->getFoundDecl()->getPrimaryTemplate();
      if (DLtemplate == nullptr)
        return;
      auto DLtemplate_name = DLtemplate->getNameAsString();
      llvm::outs() << DLtemplate_name << '\n';
      std::size_t i = 0;
      for (i = 0; i < cur_templates.size(); ++i) {
        if (cur_templates[i]->getNameAsString() == DLtemplate_name)
          break;
      }
      if (i >= cur_templates.size())
        return;
      auto DLtemplate_decl = cur_templates[i];
      auto DLtemplate_decl_content = stringutils::rangetoStr(
          *(Result.SourceManager), DLtemplate_decl->getSourceRange());
      llvm::outs() << DLtemplate_decl_content << '\n';
      DLtemplate_decl_content = "/*mut136*/" + DLtemplate_decl_content;
      DLtemplate_decl_content += "\n";
      Rewrite.ReplaceText(DLtemplate_decl->getSourceRange(),
                          DLtemplate_decl_content);
      auto DLcontent = stringutils::rangetoStr(*(Result.SourceManager),
                                               DL->getSourceRange());
      DLcontent = "/*mut136*/requires " + DLcontent;
      DLcontent += " ";
      Rewrite.ReplaceText(DL->getSourceRange(), DLcontent);
    }
}

void MutatorFrontendAction_136::MutatorASTConsumer_136::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = templateDecl().bind("Templates");
    auto concept_matcher = declRefExpr().bind("Concepts");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(concept_matcher, &callback);
    matchFinder.matchAST(Context);
}