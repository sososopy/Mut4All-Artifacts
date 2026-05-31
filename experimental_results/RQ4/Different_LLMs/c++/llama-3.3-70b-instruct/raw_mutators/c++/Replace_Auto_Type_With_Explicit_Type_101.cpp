//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Auto_Type_With_Explicit_Type_101
 */ 
class MutatorFrontendAction_101 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(101)

private:
    class MutatorASTConsumer_101 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_101(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Auto_Type_With_Explicit_Type_101.h"

// ========================================================================================================
#define MUT101_OUTPUT 1

void MutatorFrontendAction_101::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("VarDecls")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 VD->getSourceRange());
      //Check if the variable declaration uses the auto keyword
      if (VD->getType()->isAutoType()) {
        //Get the type of the initializer
        auto initializerType = VD->getInit()->getType();
        //Replace the auto keyword with the explicit type
        declaration.replace(declaration.find("auto"),