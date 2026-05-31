#pragma once
#include "Mutator_base.h"

/**
 * Move_Default_Member_Initializer_To_Constructor_184
 */ 
class MutatorFrontendAction_184 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(184)

private:
    class MutatorASTConsumer_184 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_184(Rewriter &R) : TheRewriter(R) {}
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

#include "../include/Mutator_.h"

// ========================================================================================================
#define MUT184_OUTPUT 1

void MutatorFrontendAction_184::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FieldDecl>("FieldDecl")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      
      // Check if field has a default member initializer
      if (!FD->hasInClassInitializer())
        return;
      
      // Get the initializer expression
      Expr *InitExpr = FD->getInClassInitializer();
      if (!InitExpr)
        return;
      
      // Check if initializer contains a function call (or constructor call) with an argument of the same class type
      // We'll look for a CXXConstructExpr or CallExpr that has a CXXRecordDecl of the same class
      const CXXRecordDecl *ClassDecl = dyn_cast<CXXRecordDecl>(FD->getParent());
      if (!ClassDecl)
        return;
      
      // Check if InitExpr contains a subexpression that constructs the same class type
      bool ContainsSelfConstruction = false;
      std::function<void(Expr*)> CheckExpr = [&](Expr *E) {
        if (auto *ConstructExpr = dyn_cast<CXXConstructExpr>(E)) {
          if (ConstructExpr->getConstructor()->getParent() == ClassDecl) {
            ContainsSelfConstruction = true;
            return;
          }
        }
        if (auto *CallExpr = dyn_cast<CallExpr>(E)) {
          for (unsigned i = 0; i < CallExpr->getNumArgs(); ++i) {
            CheckExpr(CallExpr->getArg(i));
          }
        }
        // Also check other expression types recursively
        if (auto *SubExpr = dyn_cast<Expr>(E)) {
          CheckExpr(SubExpr);
        }
      };
      CheckExpr(InitExpr);
      
      if (!ContainsSelfConstruction)
        return;
      
      //Get the source code text of target node
      SourceManager &SM = *Result.SourceManager;
      std::string FieldName = FD->getNameAsString();
      std::string InitText = stringutils::rangetoStr(SM, InitExpr->getSourceRange());
      
      // Remove the default member initializer from the field declaration
      SourceLocation FieldLoc = FD->getLocation();
      SourceRange InitRange = FD->getInClassInitializerRange();
      if (InitRange.isValid()) {
        // Remove the initializer part
        Rewrite.RemoveText(InitRange);
      }
      
      // Find all constructors of the class
      for (auto *Method : ClassDecl->methods()) {
        if (auto *Ctor = dyn_cast<CXXConstructorDecl>(Method)) {
          // Check if this constructor already initializes the field in its member initializer list
          bool AlreadyInitialized = false;
          for (auto *Init : Ctor->inits()) {
            if (Init->getMember() == FD) {
              AlreadyInitialized = true;
              break;
            }
          }
          
          if (!AlreadyInitialized && Ctor->hasBody()) {
            // Insert assignment at beginning of constructor body
            Stmt *Body = Ctor->getBody();
            if (Body) {
              CompoundStmt *CS = dyn_cast<CompoundStmt>(Body);
              if (CS) {
                // Insert at the beginning of the compound statement
                SourceLocation InsertLoc = CS->getLBracLoc().getLocWithOffset(1);
                std::string Assignment = FieldName + " = " + InitText + ";";
                Rewrite.InsertTextAfter(InsertLoc, "\n" + Assignment);
              }
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_184::MutatorASTConsumer_184::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = fieldDecl(hasInClassInitializer()).bind("FieldDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}