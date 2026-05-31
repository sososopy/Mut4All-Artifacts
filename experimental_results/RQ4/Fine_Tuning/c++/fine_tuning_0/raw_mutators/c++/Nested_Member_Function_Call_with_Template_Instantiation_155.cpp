//header file
#pragma once
#include "Mutator_base.h"

/**
 * Nested_Member_Function_Call_with_Template_Instantiation_155
 */ 
class MutatorFrontendAction_155 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(155)

private:
    class MutatorASTConsumer_155 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_155(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/Nested_Member_Function_Call_with_Template_Instantiation_155.h"

// ========================================================================================================
#define MUT155_OUTPUT 1

void MutatorFrontendAction_155::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("templates")) {
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getBeginLoc()))
        return;
      auto name = TD->getNameAsString();
      llvm::outs() << name << " ";
      auto specs = TD->specializations();
      for (auto spec : specs) {
        if (spec->isCompleteDefinition() == false)
          return;
        llvm::outs() << "spec"
                     << " ";
        auto methods = spec->methods();
        for (auto method : methods) {
          if (method->isImplicit())
            continue;
          if (method->isVirtual())
            continue;
          if (method->isCopyAssignmentOperator() ||
              method->isMoveAssignmentOperator())
            continue;
          if (method->hasBody() == false)
            continue;
          llvm::outs() << "method"
                       << " ";
          llvm::outs() << method->getNameAsString() << " ";
          auto method_name = method->getNameAsString();
          auto class_name = spec->getNameAsString();
          auto ins = class_name + "<int>::" + method_name + "();";
          llvm::outs() << ins << " ";
          Rewrite.ReplaceText(spec->getEndLoc(), 0, "/*mut155*/" + ins);
        }
      }
    }
}
  
void MutatorFrontendAction_155::MutatorASTConsumer_155::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = classTemplateDecl().bind("templates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}