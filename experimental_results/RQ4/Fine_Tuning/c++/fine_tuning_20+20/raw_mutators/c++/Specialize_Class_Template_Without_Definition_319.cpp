//header file
#pragma once
#include "Mutator_base.h"

/**
 * Specialize_Class_Template_Without_Definition_319
 */ 
class MutatorFrontendAction_319 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(319)

private:
    class MutatorASTConsumer_319 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_319(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::ClassTemplateSpecializationDecl *> cur_classes;
    };
};

//source file
#include "../include/specialize_class_template_without_definition_319.h"

// ========================================================================================================
#define MUT319_OUTPUT 1

void MutatorFrontendAction_319::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("Classes")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isCompleteDefinition())
        return;
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      if (DL->isStruct())
        content.replace(0, 5, "struct");
      else
        content.replace(0, 5, "class");
      cur_classes.push_back(DL);
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()), content);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>(
                   "Typename")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isCompleteDefinition())
        return;
      llvm::outs() << DL->getNameAsString() << '\n';
      for (size_t i = 0; i < cur_classes.size(); ++i) {
        if (cur_classes[i] == DL) {
          Rewrite.ReplaceText(DL->getBeginLoc(), 0,
                              "/*mut319*/using typename" + std::to_string(i) +
                                  " = int;\n");
        }
      }
    }
}
  
void MutatorFrontendAction_319::MutatorASTConsumer_319::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = classTemplateSpecializationDecl().bind("Classes");
    auto typename_matcher =
        classTemplateSpecializationDecl(hasParent(namespaceDecl())).bind(
            "Typename");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(typename_matcher, &callback);
    matchFinder.matchAST(Context);
}