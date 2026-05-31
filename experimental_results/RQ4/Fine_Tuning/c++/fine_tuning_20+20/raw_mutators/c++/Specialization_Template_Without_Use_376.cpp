//header file
#pragma once
#include "Mutator_base.h"

/**
 * Specialization_Template_Without_Use_376
 */ 
class MutatorFrontendAction_376 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(376)

private:
    class MutatorASTConsumer_376 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_376(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::ClassTemplateSpecializationDecl *> cur_sp;
    };
};

//source file
#include "../include/Specialization_Template_Without_Use_376.h"

// ========================================================================================================
#define MUT376_OUTPUT 1

void MutatorFrontendAction_376::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("Specializations")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             MT->getSourceRange());
      llvm::outs() << content << '\n';
      if (MT->getTemplateArgs().size() != 0) {
        if (MT->getTemplateArgs().size() == MT->getNumTemplateArgsAsWritten()) {
          auto left = content.find('<');
          auto right = content.rfind('>');
          if (left != string::npos && right != string::npos && left < right) {
            content.erase(left, right - left + 1);
          }
        } else {
          auto left = content.find('<');
          if (left != string::npos) {
            content.erase(left, 1);
          }
        }
      }
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(MT->getSourceRange()), content);
    }
}

void MutatorFrontendAction_376::MutatorASTConsumer_376::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = classTemplateSpecializationDecl().bind("Specializations");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}