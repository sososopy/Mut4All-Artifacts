//header file
#pragma once
#include "Mutator_base.h"

/**
 * Module_Name_Mutation_81
 */ 
class MutatorFrontendAction_81 : public MutatorFrontendAction {
    public:
      MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(81)
    
    private:
      class MutatorASTConsumer_81 : public MutatorASTConsumer {
        public:
          MutatorASTConsumer_81(Rewriter &R) : TheRewriter(R) {}
          void HandleTranslationUnit(ASTContext &Context) override;
        private:
          Rewriter &TheRewriter;
      };
      
      class Callback : public MatchFinder::MatchCallback {
        public:
          Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
          void run(const MatchFinder::MatchResult &Result) override;
        private:
          Rewriter &Rewrite;
          std::vector<const clang::ModuleDecl *> cur_modules;
      };
    };

//source file
#include "../include/module_name_mutation_81.h"

// ========================================================================================================
#define MUT81_OUTPUT 1

void MutatorFrontendAction_81::Callback::run(const MatchFinder::MatchResult &Result) {
  if (auto *MD = Result.Nodes.getNodeAs<clang::ModuleDecl>("Modules")) {
    if (!MD || !Result.Context->getSourceManager().isWrittenInMainFile(
                   MD->getLocation()))
      return;
    cur_modules.push_back(MD);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::Decl>("Decls")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;
    if (DL->getBeginLoc().isMacroID() || DL->getEndLoc().isMacroID())
      return;
    if (DL->getBeginLoc().getRawEncoding() >
        DL->getEndLoc().getRawEncoding()) // DL->getBeginLoc() should be before
                                          // DL->getEndLoc()
      return;
    if (DL->isImplicit())
      return;
    if (DL->getBeginLoc() == DL->getEndLoc())
      return;
    if (DL->getBeginLoc().isInvalid() || DL->getEndLoc().isInvalid())
      return;