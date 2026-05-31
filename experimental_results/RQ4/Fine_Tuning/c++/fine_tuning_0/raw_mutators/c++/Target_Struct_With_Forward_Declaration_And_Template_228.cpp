//header file
#pragma once
#include "Mutator_base.h"

/**
 * Target_Struct_With_Forward_Declaration_And_Template_228
 */ 
class MutatorFrontendAction_228 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(228)

private:
    class MutatorASTConsumer_228 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_228(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Target_Struct_With_Forward_Declaration_And_Template_228.h"

// ========================================================================================================
#define MUT228_OUTPUT 1

void MutatorFrontendAction_228::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isStruct() == false)
        return;
      if (DL->isCompleteDefinition() == true)
        return;
      if (DL->isTemplated() == false)
        return;
      auto DLname = DL->getNameAsString();
      auto DLtemplate = DL->getDescribedTemplate();
      if (DLtemplate == nullptr)
        return;
      auto DLtemplateparam = DLtemplate->getTemplateParameters();
      if (DLtemplateparam->size() == 0)
        return;
      auto DLtemplateparam0 = DLtemplateparam->getParam(0);
      auto DLtemplateparam0name = DLtemplateparam0->getNameAsString();
      llvm::outs() << DLname << '\n';
      auto definition =
          DLname + " { void func();};\n template<class " + DLtemplateparam0name +
          "> void " + DLname + "<" + DLtemplateparam0name + ">::func() {" +
          DLname + "<" + DLtemplateparam0name + ">* ptr; }";
      definition = "/*mut228*/" + definition;
      llvm::outs() << definition << '\n';
      Rewrite.ReplaceText(DL->getEndLoc(), 0, definition);
    }
}
  
void MutatorFrontendAction_228::MutatorASTConsumer_228::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}