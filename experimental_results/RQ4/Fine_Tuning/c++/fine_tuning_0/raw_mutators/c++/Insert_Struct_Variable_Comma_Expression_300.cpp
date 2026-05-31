//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Struct_Variable_Comma_Expression_300
 */ 
class MutatorFrontendAction_300 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(300)

private:
    class MutatorASTConsumer_300 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_300(Rewriter &R) : TheRewriter(R) {}
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
        const clang::CXXRecordDecl *target_struct;
    };
};

//source file
#include "../include/Insert_Struct_Variable_Comma_Expression_300.h"

// ========================================================================================================
#define MUT300_OUTPUT 1

void MutatorFrontendAction_300::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Struct")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (!DL->isStruct())
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      target_struct = DL;
    } else if (auto *DL =
                   Result.Nodes.getNodeAs<clang::VarDecl>("Variable")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->getType()->isStructureType())
        return;
      if (target_struct == nullptr)
        return;
      llvm::outs() << target_struct->getNameAsString() << '\n';
      auto structname = target_struct->getNameAsString();
      auto varname = DL->getNameAsString();
      string ins = "/*mut300*/" + structname + " mut300; auto mut300_2 = (mut300, " + varname + ").m;\n";
      Rewrite.ReplaceText(DL->getBeginLoc(), 0, ins);
    }
}
  
void MutatorFrontendAction_300::MutatorASTConsumer_300::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto struct_matcher = cxxRecordDecl().bind("Struct");
    auto var_matcher = varDecl().bind("Variable");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(struct_matcher, &callback);
    matchFinder.addMatcher(var_matcher, &callback);
    matchFinder.matchAST(Context);
}