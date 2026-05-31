//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Const_Qualifier_In_OMP_Atomic_Capture_347
 */ 
class MutatorFrontendAction_347 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(347)

private:
    class MutatorASTConsumer_347 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_347(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::VarDecl*> targetVars;
    };
};

//source file
#include "../include/Mutator_Insert_Const_Qualifier_In_OMP_Atomic_Capture_347.h"

// ========================================================================================================
#define MUT347_OUTPUT 1

void MutatorFrontendAction_347::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::OMPAtomicDirective>("OMPAtomic")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      //(optional)Record the node information to be used in the mutation process
      //Perform mutation on the source code text by applying string replacement
      //Replace the original AST node with the mutated one
      const OMPClause* captureClause = nullptr;
      for (const OMPClause* clause : MT->clauses()) {
        if (clause->getClauseKind() == llvm::omp::Clause::OMPC_capture) {
          captureClause = clause;
          break;
        }
      }
      if (!captureClause) return;
      
      // Find the assignment expression within the capture clause
      const Stmt* associatedSt1 = MT->getAssociatedStmt();
      if (!associatedSt1) return;
      
      // Traverse the associated statement to find assignment expressions
      std::vector<const BinaryOperator*> assignments;
      std::function<void(const Stmt*)> traverse = [&](const Stmt* S) {
        if (const BinaryOperator* BO = dyn_cast<BinaryOperator>(S)) {
          if (BO->isAssignmentOp()) {
            assignments.push_back(BO);
          }
        }
        for (const Stmt* child : S->children()) {
          if (child) traverse(child);
        }
      };
      traverse(associatedSt1);
      
      // Collect variables being written to
      for (const BinaryOperator* assign : assignments) {
        const Expr* lhs = assign->getLHS();
        if (const DeclRefExpr* DRE = dyn_cast<DeclRefExpr>(lhs)) {
          const ValueDecl* VD = DRE->getDecl();
          if (const VarDecl* varDecl = dyn_cast<VarDecl>(VD)) {
            if (!varDecl->getType().isConstQualified()) {
              targetVars.push_back(varDecl);
            }
          }
        }
      }
    }
    else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("VarDecl")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      
      // Check if this variable is in our target list
      bool isTarget = false;
      for (const VarDecl* target : targetVars) {
        if (target == VD) {
          isTarget = true;
          break;
        }
      }
      if (!isTarget) return;
      
      //Get the source code text of target node
      auto declText = stringutils::rangetoStr(*(Result.SourceManager),
                                               VD->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      // Insert const qualifier before the type
      std::string typeStr = VD->getType().getAsString();
      if (typeStr.find("const") == std::string::npos) {
        // Find the position where type name starts
        size_t typePos = declText.find(typeStr);
        if (typePos != std::string::npos) {
          declText.insert(typePos, "const ");
        }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(VD->getSourceRange()), declText);
    }
}
  
void MutatorFrontendAction_347::MutatorASTConsumer_347::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher1 = ompAtomicDirective().bind("OMPAtomic");
    DeclarationMatcher matcher2 = varDecl().bind("VarDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher1, &callback);
    matchFinder.addMatcher(matcher2, &callback);
    matchFinder.matchAST(Context);
}