//header file
#pragma once
#include "Mutator_base.h"

/**
 * Change_Array_Size_In_Omp_Array_Declaration_350
 */ 
class MutatorFrontendAction_350 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(350)

private:
    class MutatorASTConsumer_350 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_350(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::VarDecl*> arrayDecls;
        std::vector<const clang::OMPExecutableDirective*> ompDirectives;
    };
};

//source file
#include "../include/Change_Array_Size_In_Omp_Array_Declaration_350.h"

// ========================================================================================================
#define MUT350_OUTPUT 1

void MutatorFrontendAction_350::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("ArrayDecl")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      //Check if it's an array declaration with explicit size
      if (VD->getType()->isArrayType()) {
        const clang::ArrayType* AT = VD->getType()->getAsArrayTypeUnsafe();
        if (const clang::ConstantArrayType* CAT = dyn_cast<clang::ConstantArrayType>(AT)) {
          //Check if size is positive integer
          llvm::APSInt SizeVal = CAT->getSize();
          if (SizeVal.isStrictlyPositive()) {
            //Record the node information to be used in the mutation process
            arrayDecls.push_back(VD);
          }
        }
      }
    }
    else if (auto *OMP = Result.Nodes.getNodeAs<clang::OMPExecutableDirective>("OMPDirective")) {
      //Filter nodes in header files
      if (!OMP || !Result.Context->getSourceManager().isWrittenInMainFile(
                     OMP->getLocation()))
        return;
      //Record OMP directives
      ompDirectives.push_back(OMP);
    }
    else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("TargetArrayDecl")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      //Check if it's an array declaration with explicit size
      if (VD->getType()->isArrayType()) {
        const clang::ArrayType* AT = VD->getType()->getAsArrayTypeUnsafe();
        if (const clang::ConstantArrayType* CAT = dyn_cast<clang::ConstantArrayType>(AT)) {
          //Check if size is positive integer
          llvm::APSInt SizeVal = CAT->getSize();
          if (SizeVal.isStrictlyPositive()) {
            //Check if any OMP directive references this array
            bool referenced = false;
            std::string arrayName = VD->getNameAsString();
            for (const auto* OMP : ompDirectives) {
              //Check if array is referenced in OMP clauses
              for (unsigned i = 0; i < OMP->getNumClauses(); ++i) {
                const clang::OMPClause* Clause = OMP->getClause(i);
                if (Clause) {
                  //Check depend clause
                  if (const clang::OMPDependClause* DepClause = dyn_cast<clang::OMPDependClause>(Clause)) {
                    for (unsigned j = 0; j < DepClause->getNumVarLocs(); ++j) {
                      const clang::Expr* E = DepClause->getVarRef(j);
                      if (E) {
                        //Check if expression references our array
                        if (const clang::DeclRefExpr* DRE = dyn_cast<clang::DeclRefExpr>(E)) {
                          if (DRE->getDecl() == VD) referenced = true;
                        }
                      }
                    }
                  }
                  //Check map clause
                  if (const clang::OMPMapClause* MapClause = dyn_cast<clang::OMPMapClause>(Clause)) {
                    for (unsigned j = 0; j < MapClause->getNumVarLocs(); ++j) {
                      const clang::Expr* E = MapClause->getVarRef(j);
                      if (E) {
                        if (const clang::DeclRefExpr* DRE = dyn_cast<clang::DeclRefExpr>(E)) {
                          if (DRE->getDecl() == VD) referenced = true;
                        }
                      }
                    }
                  }
                  //Check reduction clause
                  if (const clang::OMPReductionClause* RedClause = dyn_cast<clang::OMPReductionClause>(Clause)) {
                    for (unsigned j = 0; j < RedClause->getNumVarLocs(); ++j) {
                      const clang::Expr* E = RedClause->getVarRef(j);
                      if (E) {
                        if (const clang::DeclRefExpr* DRE = dyn_cast<clang::DeclRefExpr>(E)) {
                          if (DRE->getDecl() == VD) referenced = true;
                        }
                      }
                    }
                  }
                }
              }
            }
            if (referenced) {
              //Get the source code text of target node
              SourceRange SR = VD->getSourceRange();
              std::string originalText = stringutils::rangetoStr(*(Result.SourceManager), SR);
              //Find the size expression
              size_t bracketPos = originalText.find('[');
              if (bracketPos != std::string::npos) {
                size_t closeBracketPos = originalText.find(']', bracketPos);
                if (closeBracketPos != std::string::npos) {
                  std::string sizeExpr = originalText.substr(bracketPos+1, closeBracketPos-bracketPos-1);
                  //Perform mutation on the source code text by applying string replacement
                  int choice = getrandom::getRandomIndex(1); // 0 for zero, 1 for negative
                  std::string newSize = (choice == 0) ? "0" : "-1";
                  std::string mutatedText = originalText;
                  mutatedText.replace(bracketPos+1, closeBracketPos-bracketPos-1, newSize);
                  //Replace the original AST node with the mutated one
                  Rewrite.ReplaceText(CharSourceRange::getTokenRange(SR), mutatedText);
                }
              }
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_350::MutatorASTConsumer_350::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher arrayMatcher = varDecl(hasType(arrayType())).bind("ArrayDecl");
    DeclarationMatcher ompMatcher = ompExecutableDirective().bind("OMPDirective");
    DeclarationMatcher targetMatcher = varDecl(hasType(arrayType())).bind("TargetArrayDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(arrayMatcher, &callback);
    matchFinder.addMatcher(ompMatcher, &callback);
    matchFinder.addMatcher(targetMatcher, &callback);
    matchFinder.matchAST(Context);
}