//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Export_Keyword_To_Template_Specialization_303
 */ 
class MutatorFrontendAction_303 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(303)

private:
    class MutatorASTConsumer_303 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_303(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::ClassTemplateDecl *> cur_templates;
    };
};

//source file
#include "../include/add_export_keyword_to_template_specialization_303.h"

// ========================================================================================================
#define MUT303_OUTPUT 1

void MutatorFrontendAction_303::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TS = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("Specializations")) {
      if (!TS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TS->getLocation()))
        return;
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), TS->getSourceRange());
      llvm::outs() << content << '\n';
      if (content.find("export") != string::npos)
        return;
      content = "export " + content;
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(TS->getSourceRange()), content);
    } else if (auto *TL = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>(
                   "Templates")) {
      if (!TL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TL->getLocation()))
        return;
      cur_templates.push_back(TL);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>(
                   "Classes")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      llvm::outs() << content << '\n';
      if (content.find("export") != string::npos)
        return;
      if (content.find("template<>") == string::npos)
        return;
      content = "export " + content;
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()), content);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>(
                   "EmptyClasses")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      llvm::outs() << content << '\n';
      if (content.find("export") != string::npos)
        return;
      if (content.find("template<>") == string::npos)
        return;
      content = "export " + content;
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()), content);
    }
}

void MutatorFrontendAction_303::MutatorASTConsumer_303::HandleTranslationUnit(
    ASTContext &Context) {
  MatchFinder Finder;
  Callback callback(TheRewriter);
  Finder.addMatcher(
      classTemplateSpecializationDecl().bind("Specializations"), &callback);
  Finder.addMatcher(classTemplateDecl().bind("Templates"), &callback);
  Finder.addMatcher(cxxRecordDecl().bind("Classes"), &callback);
  Finder.addMatcher(cxxRecordDecl(isDefinition()).bind("EmptyClasses"),
                    &callback);
  Finder.matchAST(Context);
}

std::unique_ptr<ASTConsumer>
MutatorFrontendAction_303::CreateASTConsumer(CompilerInstance &CI,
                                             StringRef file) {
  TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
  return std::make_unique<MutatorASTConsumer_303>(TheRewriter);
}