//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_NonNull_Pointer_With_Null_190
 */ 
class MutatorFrontendAction_190 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(190)

private:
    class MutatorASTConsumer_190 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_190(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::VarDecl*> candidateVarDecls;
    };
};

//source file
#include "../include/Mutator_Replace_NonNull_Pointer_With_Null_190.h"

// ========================================================================================================
#define MUT190_OUTPUT 1

void MutatorFrontendAction_190::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("varDecl")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      //Check if variable is pointer/reference to class/struct
      QualType QT = VD->getType();
      if (!QT->isPointerType() && !QT->isReferenceType())
        return;
      const Type* PT = QT->getPointeeType().getTypePtrOrNull();
      if (!PT || !PT->isRecordType())
        return;
      //Record candidate variable declarations
      candidateVarDecls.push_back(VD);
    }
    else if (auto *ME = Result.Nodes.getNodeAs<clang::MemberExpr>("memberExpr")) {
      //Filter nodes in header files
      if (!ME || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ME->getLocation()))
        return;
      //Check if member expression dereferences a pointer/reference
      Expr* Base = ME->getBase();
      if (!Base)
        return;
      QualType BaseQT = Base->getType();
      if (!BaseQT->isPointerType() && !BaseQT->isReferenceType())
        return;
      //Find if base is one of candidate variables
      const VarDecl* BaseVD = nullptr;
      if (auto* DRE = dyn_cast<DeclRefExpr>(Base)) {
        BaseVD = dyn_cast<VarDecl>(DRE->getDecl());
      }
      if (!BaseVD)
        return;
      bool isCandidate = false;
      for (const VarDecl* VD : candidateVarDecls) {
        if (VD == BaseVD) {
          isCandidate = true;
          break;
        }
      }
      if (!isCandidate)
        return;
      //Get the source code text of target node (the variable declaration)
      SourceRange VDRange = BaseVD->getSourceRange();
      //Perform mutation on the source code text by applying string replacement
      //Replace initialization expression with nullptr
      if (BaseVD->hasInit()) {
        Expr* InitExpr = BaseVD->getInit();
        SourceRange InitRange = InitExpr->getSourceRange();
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(InitRange, "nullptr");
      }
    }
}
  
void MutatorFrontendAction_190::MutatorASTConsumer_190::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher varMatcher = varDecl().bind("varDecl");
    StatementMatcher memberMatcher = memberExpr().bind("memberExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(varMatcher, &callback);
    matchFinder.addMatcher(memberMatcher, &callback);
    matchFinder.matchAST(Context);
}