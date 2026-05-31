//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_Specialization_With_Partial_214
 */ 
class MutatorFrontendAction_214 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(214)

private:
    class MutatorASTConsumer_214 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_214(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::ClassTemplateDecl *> templateDecls;
        std::vector<const clang::ClassTemplateSpecializationDecl *> specializationDecls;
    };
};

//source file
#include "../include/Mutator_214.h"

// ========================================================================================================
#define MUT214_OUTPUT 1

void MutatorFrontendAction_214::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CTD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateDecl")) {
      //Filter nodes in header files
      if (!CTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CTD->getLocation()))
        return;
      //Record the node information to be used in the mutation process
      templateDecls.push_back(CTD);
    }
    else if (auto *CTSD = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("SpecializationDecl")) {
      //Filter nodes in header files
      if (!CTSD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CTSD->getLocation()))
        return;
      if (!CTSD->isExplicitSpecialization())
        return;
      //Record the node information to be used in the mutation process
      specializationDecls.push_back(CTSD);
    }
    else if (auto *CTSD = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("TargetSpecialization")) {
      //Filter nodes in header files
      if (!CTSD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CTSD->getLocation()))
        return;
      if (!CTSD->isExplicitSpecialization())
        return;
      //Get the source code text of target node
      auto specializationSource = stringutils::rangetoStr(*(Result.SourceManager), CTSD->getSourceRange());
      //Find matching template declaration
      const clang::ClassTemplateDecl *matchingTemplate = nullptr;
      for (auto *CTD : templateDecls) {
        if (CTD->getQualifiedNameAsString() == CTSD->getQualifiedNameAsString()) {
          matchingTemplate = CTD;
          break;
        }
      }
      if (!matchingTemplate)
        return;
      //Find other specializations for the same template
      const clang::ClassTemplateSpecializationDecl *otherSpecialization = nullptr;
      for (auto *spec : specializationDecls) {
        if (spec->getQualifiedNameAsString() == CTSD->getQualifiedNameAsString() && spec != CTSD) {
          otherSpecialization = spec;
          break; 
        }
      }
      if (!otherSpecialization)
        return;
      //Perform mutation on the source code text by applying string replacement
      std::string partial