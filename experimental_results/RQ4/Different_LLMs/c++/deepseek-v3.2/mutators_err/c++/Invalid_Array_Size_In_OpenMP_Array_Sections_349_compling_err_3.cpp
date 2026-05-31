//header file
#pragma once
#include "Mutator_base.h"

/**
 * Invalid_Array_Size_In_OpenMP_Array_Sections_349
 */ 
class MutatorFrontendAction_349 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(349)

private:
    class MutatorASTConsumer_349 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_349(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::VarDecl*> candidateArrays;
    };
};

//source file
#include "../include/Invalid_Array_Size_In_OpenMP_Array_Sections_349.h"

// ========================================================================================================
#define MUT349_OUTPUT 1

void MutatorFrontendAction_349::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("arrayDecl")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      //Check if it's an array declaration with constant size
      if (VD->getType()->isArrayType()) {
        const clang::ArrayType* AT = VD->getType()->getAsArrayTypeUnsafe();
        if (const clang::ConstantArrayType* CAT = dyn_cast<clang::ConstantArrayType>(AT)) {
          //Record candidate arrays
          candidateArrays.push_back(VD);
        }
      }
    }
    else if (auto *OS = Result.Nodes.getNodeAs<clang::ArraySubscriptExpr>("ompArraySection")) {
      //Filter nodes in header files
      if (!OS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     OS->getBeginLoc()))
        return;
      //Get the base expression
      const clang::Expr* Base = OS->getBase();
      if (!Base) return;
      
      //Try to find the referenced array declaration
      clang::DeclRefExpr* DRE = dyn_cast<clang::DeclRefExpr>(Base);
      if (!DRE) return;
      clang::VarDecl* ArrayVar = dyn_cast<clang::VarDecl>(DRE->getDecl());
      if (!ArrayVar) return;
      
      //Find this array in our candidate list
      clang::VarDecl* TargetDecl = nullptr;
      for (auto* Cand : candidateArrays) {
        if (Cand == ArrayVar) {
          TargetDecl = const_cast<clang::VarDecl*>(Cand);
          break;
        }
      }
      if (!TargetDecl) return;
      
      //Get the source code text of target node
      SourceRange DeclRange = TargetDecl->getSourceRange();
      std::string OriginalDecl = stringutils::rangetoStr(*(Result.SourceManager), DeclRange);
      
      //Perform mutation on the source code text by applying string replacement
      //Find the array size part
      size_t OpenBracket = OriginalDecl.find('[');
      if (OpenBracket == std::string::npos) return;
      size_t CloseBracket = OriginalDecl.find(']', OpenBracket);
      if (CloseBracket == std::string::npos) return;
      
      std::string SizePart = OriginalDecl.substr(OpenBracket + 1, CloseBracket - OpenBracket - 1);
      std::string NewSize;
      int choice = getrandom::getRandomIndex(2);
      if (choice == 0) {
        NewSize = "-1";
      } else if (choice == 1) {
        NewSize = "0";
      } else {
        //Find an existing integer variable in the same scope
        clang::DeclContext* Context = TargetDecl->getDeclContext();
        std::vector<std::string> IntVarNames;
        for (auto* Decl : Context->decls()) {
          if (auto* Var = dyn_cast<clang::VarDecl>(Decl)) {
            if (Var->getType()->isIntegerType() && Var->getNameAsString() != "") {
              IntVarNames.push_back(Var->getNameAsString());
            }
          }
        }
        if (IntVarNames.empty()) {
          NewSize = "0"; //Fallback
        } else {
          int idx = getrandom::getRandomIndex(IntVarNames.size() - 1);
          NewSize = IntVarNames[idx];
        }
      }
      
      std::string MutatedDecl = OriginalDecl;
      MutatedDecl.replace(OpenBracket + 1, SizePart.length(), NewSize);
      MutatedDecl = "/*mut349*/" + MutatedDecl;
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(DeclRange), MutatedDecl);
    }
}
  
void MutatorFrontendAction_349::MutatorASTConsumer_349::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher arrayMatcher = varDecl(hasType(arrayType())).bind("arrayDecl");
    StatementMatcher ompMatcher = arraySubscriptExpr().bind("ompArraySection");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(arrayMatcher, &callback);
    matchFinder.addMatcher(ompMatcher, &callback);
    matchFinder.matchAST(Context);
}