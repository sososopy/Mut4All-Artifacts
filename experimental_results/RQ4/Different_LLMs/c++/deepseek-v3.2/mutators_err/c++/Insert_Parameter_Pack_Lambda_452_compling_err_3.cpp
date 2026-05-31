//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Parameter_Pack_Lambda_452
 */ 
class MutatorFrontendAction_452 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(452)

private:
    class MutatorASTConsumer_452 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_452(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Insert_Parameter_Pack_Lambda_452.h"

// ========================================================================================================
#define MUT452_OUTPUT 1

void MutatorFrontendAction_452::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Check if it's a template function with parameter pack
      if (!FD->isTemplated() || !FD->getTemplateSpecializationInfo())
        return;
      const TemplateArgumentList *TAL = FD->getTemplateSpecializationInfo()->TemplateArguments;
      if (!TAL || TAL->size() == 0)
        return;
      bool hasParameterPack = false;
      for (unsigned i = 0; i < TAL->size(); ++i) {
        if (TAL->get(i).getKind() == TemplateArgument::Pack)
          hasParameterPack = true;
      }
      if (!hasParameterPack)
        return;
      //Get the source code text of target node
      Stmt *Body = FD->getBody();