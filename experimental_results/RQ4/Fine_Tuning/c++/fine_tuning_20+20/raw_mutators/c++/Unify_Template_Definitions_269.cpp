//header file
#pragma once
#include "Mutator_base.h"

/**
 * Unify_Template_Definitions_269
 */ 
class MutatorFrontendAction_269 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(269)

private:
    class MutatorASTConsumer_269 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_269(Rewriter &R) : TheRewriter(R) {}
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
        const clang::ClassTemplateDecl *target_template = nullptr;
        std::vector<const clang::CXXMethodDecl *> target_methods;
    };
};

//source file
#include "../include/Unify_Template_Definitions_269.h"

// ========================================================================================================
#define MUT269_OUTPUT 1

void MutatorFrontendAction_269::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Template")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      target_template = MT;
    } else if (auto *MD =
                   Result.Nodes.getNodeAs<clang::CXXMethodDecl>("Methods")) {
      if (!MD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MD->getLocation()))
        return;
      target_methods.push_back(MD);
    }
  }

void MutatorFrontendAction_269::MutatorASTConsumer_269::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = classTemplateDecl().bind("Template");
    auto method_matcher = cxxMethodDecl().bind("Methods");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(method_matcher, &callback);
    matchFinder.matchAST(Context);
    if (callback.target_template == nullptr)
      return;
    auto DL = callback.target_template;
    std::string DL_name = DL->getNameAsString();
    std::vector<const clang::CXXMethodDecl *> target_methods;
    for (auto method : callback.target_methods) {
      if (method->getQualifiedNameAsString().find(DL_name) != std::string::npos)
        target_methods.push_back(method);
    }
    if (target_methods.empty())
      return;
    auto DL_content = stringutils::rangetoStr(*(DL->getASTContext().getSourceManager()),
                                              DL->getSourceRange());
    llvm::outs() << DL_content << '\n';
    for (auto method : target_methods) {
      auto method_content = stringutils::rangetoStr(
          *(method->getASTContext().getSourceManager()), method->getSourceRange());
      llvm::outs() << method_content << '\n';
      DL_content.insert(DL_content.rfind('}'), method_content);
    }
    Rewrite.ReplaceText(
        CharSourceRange::getTokenRange(DL->getSourceRange()), DL_content);
    for (auto method : target_methods) {
      Rewrite.ReplaceText(method->getBeginLoc(), 2, "/*mut269*/");
    }
}