//header file
#pragma once
#include "Mutator_base.h"

/**
 * Union_With_Flexible_Array_Member_484
 */ 
class MutatorFrontendAction_484 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(484)

private:
    class MutatorASTConsumer_484 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_484(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Union_With_Flexible_Array_Member_484.h"

// ========================================================================================================
#define MUT484_OUTPUT 1

void MutatorFrontendAction_484::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("varDecl")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      //Check if the variable is of union type
      const Type *T = VD->getType().getTypePtr();
      if (!T || !T->isUnionType())
        return;
      //Check if the union contains a struct with flexible array member
      const RecordDecl *RD = T->getAsRecordDecl();
      if (!RD)
        return;
      bool hasFAMStruct = false;
      for (const FieldDecl *FD : RD->fields()) {
        const Type *FT = FD->getType().getTypePtr();
        if (FT && FT->isStructureType()) {
          const RecordDecl *StructRD = FT->getAsRecordDecl();
          if (StructRD && StructRD->isStruct()) {
            const FieldDecl *LastField = nullptr;
            for (const FieldDecl *StructFD : StructRD->fields()) {
              LastField = StructFD;
            }
            if (LastField && LastField->getType()->isArrayType()) {
              const ArrayType *AT = LastField->getType()->getAsArrayType();
              if (AT && AT->isIncompleteArrayType()) {
                hasFAMStruct = true;
                break;
              }
            }
          }
        }
      }
      if (!hasFAMStruct)
        return;
      //Check if the union is not a member of another aggregate
      if (VD->getDeclContext()->isRecord())
        return;
      //Get the source code text of target node
      SourceRange SR = VD->getSourceRange();
      //Perform mutation on the source code text by applying string replacement
      SourceLocation InitLoc = VD->getInit() ? VD->getInit()->getBeginLoc() : VD->getEndLoc();
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(SourceRange(InitLoc, InitLoc)), "{}");
    }
}
  
void MutatorFrontendAction_484::MutatorASTConsumer_484::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl().bind("varDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}