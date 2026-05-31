//header file
#pragma once
#include "Mutator_base.h"

/**
 * Variadic_Template_Instantiation_9
 */ 
class MutatorFrontendAction_9 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(9)

private:
    class MutatorASTConsumer_9 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_9(Rewriter &R) : TheRewriter(R) {}
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
        const clang::CXXRecordDecl *cur_class;
    };
};

//source file
#include "../include/Variadic_Template_Instantiation_9.h"

// ========================================================================================================
#define MUT9_OUTPUT 1

void MutatorFrontendAction_9::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Classes")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->getTemplateParameters()->size() != 1)
        return;
      if (DL->getTemplateParameters()->getParam(0)->isTemplateParameterPack() ==
          false)
        return;
      if (DL->getTemplateParameters()
              ->getParam(0)
              ->getTypeForDecl()
              ->isDependentType() == false)
        return;
      // DL->dump();
      cur_class = DL->getTemplatedDecl();
      llvm::outs() << cur_class->getNameAsString() << '\n';
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      auto ins = "\nvoid f() { int x = sizeof...(" +
                 DL->getTemplateParameters()->getParam(0)->getNameAsString() +
                 "); }";
      if (content.rfind('}') != string::npos)
        content.insert(content.rfind('}'), ins);
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(DL->getSourceRange()), content);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>(
                   "Classes_without_template")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->getNameAsString() != cur_class->getNameAsString())
        return;
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      auto ins = "<int, char, double>";
      if (content.find('}') != string::npos)
        content.insert(content.find('}'), ins);
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(DL->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_9::MutatorASTConsumer_9::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = classTemplateDecl().bind("Classes");
    auto matcher2 = cxxRecordDecl().bind("Classes_without_template");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(matcher2, &callback);
    matchFinder.matchAST(Context);
}