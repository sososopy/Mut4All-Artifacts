//header file
#pragma once
#include "Mutator_base.h"

/**
 * Delete_Constexpr_Member_Function_15
 */ 
class MutatorFrontendAction_15 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(15)

private:
    class MutatorASTConsumer_15 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_15(Rewriter &R) : TheRewriter(R) {}
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
        
    };
};

//source file
#include "../include/Delete_Constexpr_Member_Function_15.h"

// ========================================================================================================
#define MUT15_OUTPUT 1

void MutatorFrontendAction_15::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("Methods")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      if (MT->isDeleted())
        return;
      if (MT->isConstexpr()) {
        auto decl = stringutils::rangetoStr(*(Result.SourceManager),
                                            MT->getSourceRange());
        decl += "=delete";
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()),
                            decl);
      }
    } else if (auto *DL =
                   Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      auto methods = DL->methods();
      for (auto method : methods) {
        if (method->isDeleted())
          return;
      }
      for (auto method : methods) {
        if (method->getReturnType().getAsString() == "void" ||
            method->getReturnType().getAsString() == "int") {
          auto decl = stringutils::rangetoStr(*(Result.SourceManager),
                                              method->getSourceRange());
          decl += "=delete";
          Rewrite.ReplaceText(
              CharSourceRange::getTokenRange(method->getSourceRange()), decl);
        }
      }
    }
}
  
void MutatorFrontendAction_15::MutatorASTConsumer_15::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxMethodDecl().bind("Methods");
    auto class_matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(class_matcher, &callback);
    matchFinder.matchAST(Context);
}