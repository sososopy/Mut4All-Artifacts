//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Static_Member_Template_Arg_With_Invalid_Decltype_205
 */ 
class MutatorFrontendAction_205 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(205)

private:
    class MutatorASTConsumer_205 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_205(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl *> currentClasses;
    };
};

//source file
#include "../include/Mutator_Replace_Static_Member_Template_Arg_With_Invalid_Decltype_205.h"

// ========================================================================================================
#define MUT205_OUTPUT 1

void MutatorFrontendAction_205::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("ClassDecl")) {
      //Filter nodes in header files
      if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CL->getLocation()))
        return;
      if (!CL->isCompleteDefinition())
        return;
      //Record the node information to be used in the mutation process
      currentClasses.push_back(CL);
    }
    else if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("StaticCall")) {
      //Filter nodes in header files
      if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CE->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto *DRE = dyn_cast<clang::DeclRefExpr>(CE->getCallee()->IgnoreImpCasts());
      if (!DRE)
        return;
      auto *FD = dyn_cast<clang::FunctionDecl>(DRE->getDecl());
      if (!FD || !FD->isStatic())
        return;
      
      // Check if this is a template call with explicit template arguments
      auto *Callee = CE->getCallee();
      if (auto *ULE = dyn_cast<clang::UnresolvedLookupExpr>(Callee)) {
        if (!ULE->hasExplicitTemplateArgs())
          return;
      } else if (auto *DRE = dyn_cast<clang::DeclRefExpr>(Callee)) {
        auto *ND = DRE->getDecl();
        if (auto *FTD = dyn_cast<clang::FunctionTemplateDecl>(ND)) {
          // Has template arguments
        } else if (auto *FD = dyn_cast<clang::FunctionDecl>(ND)) {
          if (!FD->getTemplateSpecializationInfo())
            return;
        } else {
          return;
        }
      } else {
        return;
      }
      
      // Find the enclosing class
      const clang::CXXRecordDecl *EnclosingClass = nullptr;
      auto *DC = Result.Context->getParents(*CE)[0].get<clang::Decl>();
      while (DC) {
        if (auto *RD = dyn_cast<clang::CXXRecordDecl>(DC)) {
          EnclosingClass = RD;
          break;
        }
        DC = Result.Context->getParents(*DC)[0].get<clang::Decl>();
      }
      
      if (!EnclosingClass)
        return;
      
      // Get the source range of the template arguments
      SourceRange TemplateArgRange;
      if (auto *ULE = dyn_cast<clang::UnresolvedLookup