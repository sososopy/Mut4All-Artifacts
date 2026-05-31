//header file
#pragma once
#include "Mutator_base.h"

/**
 * Toggle_Dependent_Name_Qualification_170
 */ 
class MutatorFrontendAction_170 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(170)

private:
    class MutatorASTConsumer_170 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_170(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Toggle_Dependent_Name_Qualification_170.h"

// ========================================================================================================
#define MUT170_OUTPUT 1

void MutatorFrontendAction_170::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *DRE = Result.Nodes.getNodeAs<clang::DeclRefExpr>("DependentName")) {
      //Filter nodes in header files
      if (!DRE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DRE->getLocation()))
        return;

      // Check if we are inside a member function of a class template
      const auto *FuncDecl = Result.Context->getEnclosingFunctionDecl(DRE);
      if (!FuncDecl) return;
      const auto *MethodDecl = llvm::dyn_cast<clang::CXXMethodDecl>(FuncDecl);
      if (!MethodDecl) return;
      const auto *RecordDecl = MethodDecl->getParent();
      if (!RecordDecl || !RecordDecl->getDescribedClassTemplate()) return;

      // Check if the class template has a dependent base class
      bool hasDependentBase = false;
      for (const auto &Base : RecordDecl->bases()) {
        if (Base.getType()->isDependentType()) {
          hasDependentBase = true;
          break;
        }
      }
      if (!hasDependentBase) return;

      // Check that the referenced declaration is a member of a dependent base
      const auto *Decl = DRE->getDecl();
      if (!Decl) return;
      const auto *MemberDecl = llvm::dyn_cast<clang::ValueDecl>(Decl);
      if (!MemberDecl) return;
      const auto *MemberRecord = llvm::dyn_cast<clang::CXXRecordDecl>(MemberDecl->getDeclContext());
      if (!MemberRecord) return;
      
      // Ensure MemberRecord is a base class of RecordDecl
      bool isBase = false;
      for (const auto &Base : RecordDecl->bases()) {
        if (Base.getType()->getAsCXXRecordDecl() == MemberRecord) {
          isBase = true;
          break;
        }
      }
      if (!isBase) return;

      // Ensure the name is not already qualified with a scope resolution operator (other than this->)
      if (auto *Qualifier = DRE->getQualifier()) {
        if (!llvm::isa<clang::CXXThisExpr>(Qualifier)) return;
      }

      //Get the source code text of target node
      std::string NameStr = DRE->getNameInfo().getName().getAsString();
      clang::SourceRange ReplaceRange = DRE->getSourceRange();
      
      //Perform mutation on the source code text by applying string replacement
      std::string Replacement;
      // Randomly choose to add this-> or remove it if present
      int choice = getrandom::getRandomIndex(1);
      
      // Check if currently has this->
      bool currentlyHasThis = false;
      if (auto *Parent = llvm::dyn_cast<clang::MemberExpr>(DRE->IgnoreParens())) {
        if (llvm::isa<clang::CXXThisExpr>(Parent->getBase()->IgnoreParenImpCasts())) {
          currentlyHasThis = true;
        }
      }
      
      if (currentlyHasThis) {
        // Remove this-> qualification
        // We need to get the full member expression source range
        if (auto *ME = llvm::dyn_cast<clang::MemberExpr>(DRE->IgnoreParens())) {
          ReplaceRange = ME->getSourceRange();
          Replacement = NameStr;
        }
      } else {
        // Add this-> qualification
        if (choice == 0) {
          Replacement = "this->" + NameStr;
        } else {
          // Use base class name qualification
          Replacement = MemberRecord->getNameAsString() + "::" + NameStr;
        }
      }
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(ReplaceRange, Replacement);
    }
}
  
void MutatorFrontendAction_170::MutatorASTConsumer_170::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = declRefExpr(
      hasAncestor(cxxMethodDecl(
        hasParent(cxxRecordDecl(
          hasDescendant(classTemplateDecl())
        ))
      )),
      unless(hasAncestor(namespaceDecl())),
      unless(hasAncestor(functionDecl(unless(cxxMethodDecl())))),
      unless(isExpansionInSystemHeader())
    ).bind("DependentName");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}