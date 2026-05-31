//header file
#pragma once
#include "Mutator_base.h"

/**
 * Constrain_Nested_NTTP_in_Template_Class_Scope_94
 */ 
class MutatorFrontendAction_94 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(94)

private:
    class MutatorASTConsumer_94 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_94(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::ClassTemplateDecl *> outer_templates;
    };
};

//source file
#include "../include/Constrain_Nested_NTTP_in_Template_Class_Scope_94.h"

// ========================================================================================================
#define MUT94_OUTPUT 1

void MutatorFrontendAction_94::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Outer")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getBeginLoc()))
      return;
    outer_templates.push_back(DL);
  } else if (auto *DL =
                 Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Nested")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getBeginLoc()))
      return;
    if (DL->getNameAsString().find("mut_94") != string::npos)
      return;
    if (outer_templates.empty())
      return;
    auto outer = outer_templates.back();
    if (!DL->getBeginLoc().isMacroID()) {
      if (DL->getBeginLoc() < outer->getBeginLoc() ||
          DL->getEndLoc() > outer->getEndLoc())
        return;
    }
    auto content =
        stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
    auto nttp = "template<int mut_94> ";
    content.insert(content.find("class "), nttp);
    Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()),
                        content);
  }
}
  
void MutatorFrontendAction_94::MutatorASTConsumer_94::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto outer_matcher = classTemplateDecl().bind("Outer");
  auto nested_matcher = classTemplateDecl(hasParent(classTemplateDecl()))
                            .bind("Nested");
  Callback callback(TheRewriter);
  matchFinder.addMatcher(outer_matcher, &callback);
  matchFinder.addMatcher(nested_matcher, &callback);
  matchFinder.matchAST(Context);
}