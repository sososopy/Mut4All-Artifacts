//header file
#pragma once
#include "Mutator_base.h"

/**
 * Alter_Nested_Name_Specifier_In_Pointer_To_Member_155
 */ 
class MutatorFrontendAction_155 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(155)

private:
    class MutatorASTConsumer_155 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_155(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::NamedDecl*> availableDecls;
    };
};

//source file
#include "../include/Mutator_Alter_Nested_Name_Specifier_In_Pointer_To_Member_155.h"

// ========================================================================================================
#define MUT155_OUTPUT 1

void MutatorFrontendAction_155::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *ND = Result.Nodes.getNodeAs<clang::NamedDecl>("NamedDecl")) {
      //Filter nodes in header files
      if (!ND || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ND->getLocation()))
        return;
      //Record available declarations for context lookup
      if (ND->getIdentifier() && !ND->getName().empty()) {
        availableDecls.push_back(ND);
      }
    }
    else if (auto *MPT = Result.Nodes.getNodeAs<clang::MemberPointerType>("MemberPointerType")) {
      //Filter nodes in header files
      if (!MPT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MPT->getTypeLoc().getBeginLoc()))
        return;
      
      //Get the source code text of target node
      auto &SM = *Result.SourceManager;
      auto &LO = Result.Context->getLangOpts();
      clang::TypeLoc TL = MPT->getTypeLoc();
      if (!TL) return;
      
      //Find the nested-name-specifier location
      clang::MemberPointerTypeLoc MPTL = TL.getAs<clang::MemberPointerTypeLoc>();
      if (!MPTL) return;
      
      clang::QualType ClassType = MPTL.getClass();
      clang::TypeLoc ClassTL = MPTL.getClassLoc();
      if (!ClassTL) return;
      
      //Get the original class/namespace name
      std::string OriginalName;
      if (auto *RT = ClassType->getAs<clang::RecordType>()) {
        OriginalName = RT->getDecl()->getNameAsString();
      } else if (auto *NT = ClassType->getAs<clang::NamespaceType>()) {
        OriginalName = NT->getDecl()->getNameAsString();
      } else {
        return;
      }
      
      //Look for enclosing context (B)
      std::string EnclosingName;
      for (auto *Decl : availableDecls) {
        if (auto *RD = llvm::dyn_cast<clang::RecordDecl>(Decl)) {
          if (RD->getNameAsString() == OriginalName) {
            if (auto *Parent = llvm::dyn_cast<clang::NamedDecl>(RD->getDeclContext())) {
              if (Parent->getIdentifier()) {
                EnclosingName = Parent->getNameAsString();
                break;
              }
            }
          }
        } else if (auto *NSD = llvm::dyn_cast<clang::NamespaceDecl>(Decl)) {
          if (NSD->getNameAsString() == OriginalName) {
            if (auto *Parent = llvm::dyn_cast<clang::NamedDecl>(NSD->getDeclContext())) {
              if (Parent->getIdentifier() && !Parent->isTranslationUnit()) {
                EnclosingName = Parent->getNameAsString();
                break;
              }
            }
          }
        }
      }
      
      //If no enclosing context found, look for nested context (C)
      std::string NestedName;
      if (EnclosingName.empty()) {
        for (auto *Decl : availableDecls) {
          if (auto *RD = llvm::dyn_cast<clang::RecordDecl>(Decl)) {
            if (RD->getDeclContext()->isRecord()) {
              if (auto *Parent = llvm::dyn_cast<clang::RecordDecl>(RD->getDeclContext())) {
                if (Parent->getNameAsString() == OriginalName) {
                  NestedName = RD->getNameAsString();
                  break;
                }
              }
            }
          } else if (auto *NSD = llvm::dyn_cast<clang::NamespaceDecl>(Decl)) {
            if (NSD->getDeclContext()->isNamespace()) {
              if (auto *Parent = llvm::dyn_cast<clang::NamespaceDecl>(NSD->getDeclContext())) {
                if (Parent->getNameAsString() == OriginalName) {
                  NestedName = NSD->getNameAsString();
                  break;
                }
              }
            }
          }
        }
      }
      
      //Perform mutation on the source code text by applying string replacement
      std::string Replacement;
      if (!EnclosingName.empty()) {
        // Mutate to B::A::*ptr
        Replacement = EnclosingName + "::" + OriginalName;
      } else if (!NestedName.empty()) {
        // Mutate to A::C::*ptr
        Replacement = OriginalName + "::" + NestedName;
      } else {
        return; // No mutation possible
      }
      
      //Get the source range of the class name in the pointer-to-member
      clang::SourceRange ReplaceRange = ClassTL.getSourceRange();
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(ReplaceRange, Replacement);
    }
}
  
void MutatorFrontendAction_155::MutatorASTConsumer_155::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher declMatcher = namedDecl(unless(isImplicit())).bind("NamedDecl");
    TypeMatcher typeMatcher = memberPointerType().bind("MemberPointerType");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(declMatcher, &callback);
    matchFinder.addMatcher(typeMatcher, &callback);
    matchFinder.matchAST(Context);
}