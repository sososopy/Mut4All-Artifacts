//header file
#pragma once
#include "Mutator_base.h"

/**
 * Inject_Malformed_Member_After_Template_Dependent_340
 */ 
class MutatorFrontendAction_340 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(340)

private:
    class MutatorASTConsumer_340 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_340(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::FieldDecl*> candidateFields;
    };
};

//source file
#include "../include/Mutator_Inject_Malformed_Member_After_Template_Dependent_340.h"

// ========================================================================================================
#define MUT340_OUTPUT 1

void MutatorFrontendAction_340::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FieldDecl>("templateDependentField")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      
      // Check if field type involves template specialization or decltype
      auto QT = FD->getType();
      if (QT.isNull()) return;
      
      bool isTemplateSpecialization = false;
      bool isDecltype = false;
      
      // Check for template specialization
      if (QT->getAs<clang::TemplateSpecializationType>()) {
        isTemplateSpecialization = true;
      }
      // Check for decltype
      else if (QT->getAs<clang::DecltypeType>()) {
        isDecltype = true;
      }
      // Check for elaborated type (like SomeTemplate<int>)
      else if (auto *ET = QT->getAs<clang::ElaboratedType>()) {
        auto *NT = ET->getNamedType()->getAs<clang::TemplateSpecializationType>();
        if (NT) isTemplateSpecialization = true;