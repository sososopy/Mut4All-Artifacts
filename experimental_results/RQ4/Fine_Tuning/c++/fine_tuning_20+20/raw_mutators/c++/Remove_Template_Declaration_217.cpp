//header file
#pragma once
#include "Mutator_base.h"

/**
 * Remove_Template_Declaration_217
 */ 
class MutatorFrontendAction_217 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(217)

private:
    class MutatorASTConsumer_217 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_217(Rewriter &R) : TheRewriter(R) {}
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
        const clang::TemplateDecl *last_template_decl=nullptr;
    };
};

//source file
#include "../include/Remove_Template_Declaration_217.h"

// ========================================================================================================
#define MUT217_OUTPUT 1

void MutatorFrontendAction_217::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::TemplateDecl>("Templates")) {
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getBeginLoc()))
        return;
      if (last_template_decl == nullptr)
        last_template_decl = TD;
    } else if (auto *TD = Result.Nodes.getNodeAs<clang::TemplateDecl>(
                   "LastTemplate")) {
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getBeginLoc()))
        return;
      if (TD != last_template_decl)
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             TD->getSourceRange());
      llvm::outs() << content << '\n';
      if (content.find("template") != string::npos) {
        content.erase(content.find("template"), content.find('>') + 1);
        llvm::outs() << content << '\n';
      }
      if (content.find("typename") != string::npos) {
        content.replace(content.find("typename"), 8, "int");
        llvm::outs() << content << '\n';
      }
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(TD->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_217::MutatorASTConsumer_217::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = templateDecl().bind("Templates");
    auto last_matcher = templateDecl().bind("LastTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(last_matcher, &callback);
    matchFinder.matchAST(Context);
}