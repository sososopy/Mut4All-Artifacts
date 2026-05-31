//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Int_Const_With_Template_Param_185
 */ 
class MutatorFrontendAction_185 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(185)

private:
    class MutatorASTConsumer_185 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_185(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::TemplateTypeParmDecl *> templateParams;
    };
};

//source file
#include "../include/Mutator_Replace_Int_Const_With_Template_Param_185.h"

// ========================================================================================================
#define MUT185_OUTPUT 1

void MutatorFrontendAction_185::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TP = Result.Nodes.getNodeAs<clang::TemplateTypeParmDecl>("TemplateParam")) {
      //Filter nodes in header files
      if (!TP || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TP->getLocation()))
        return;
      //Record the node information to be used in the mutation process
      templateParams.push_back(TP);
    }
    else if (auto *BC = Result.Nodes.getNodeAs<clang::BinaryOperator>("BinaryComp")) {
      //Filter nodes in header files
      if (!BC || !Result.Context->getSourceManager().isWrittenInMainFile(
                     BC->getOperatorLoc()))
        return;
      if (!BC->isComparisonOp())
        return;
    }
}