//header file
#pragma once
#include "Mutator_base.h"

/**
 * Reference_Type_Mismatch_157
 */ 
class MutatorFrontendAction_157 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(157)

private:
    class MutatorASTConsumer_157 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_157(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/reference_type_mismatch_157.h"

// ========================================================================================================
#define MUT157_OUTPUT 1

void MutatorFrontendAction_157::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *DL = Result.Nodes.getNodeAs<clang::DeclRefExpr>("DeclRef")) {
      //Filter nodes in header files
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      //Get the source code text of target node
      auto DL_name = DL->getNameInfo().getAsString();
      if (auto *VD = DL->getDecl()->getAs<VarDecl>()) {
        if (VD->getType()->isReferenceType() == false)
          return;
        if (VD->isLocalVarDecl() == false)
          return;
        llvm::outs() << DL_name << '\n';
        auto DL_type = stringutils::rangetoStr(*(Result.SourceManager),
                                               VD->getTypeSourceInfo()
                                                   ->getTypeLoc()
                                                   .getSourceRange());
        DL_type = stringutils::removeSpace(DL_type);
        DL_type = stringutils::removeConst(DL_type);
        DL_type = stringutils::removeReference(DL_type);
        DL_name = DL_type + " " + DL_name;
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(DL->getSourceRange(), DL_name);
    }
}

void MutatorFrontendAction_157::MutatorASTConsumer_157::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = declRefExpr().bind("DeclRef");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}