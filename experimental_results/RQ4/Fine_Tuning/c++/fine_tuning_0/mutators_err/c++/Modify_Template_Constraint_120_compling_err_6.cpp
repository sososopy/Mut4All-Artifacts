//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Constraint_120
 */ 
class MutatorFrontendAction_120 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(120)

private:
    class MutatorASTConsumer_120 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_120(Rewriter &R) : TheRewriter(R) {}
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
        
    };
};

//source file
#include "../include/Modify_Template_Constraint_120.h"

// ========================================================================================================
#define MUT120_OUTPUT 1

void MutatorFrontendAction_120::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::TemplateDecl>("Templates")) {
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;
      if (TD->getKind() != Decl::Kind::ClassTemplate &&
          TD->getKind() != Decl::Kind::FunctionTemplate)
        return;
      if (TD->getKind() == Decl::Kind::ClassTemplate) {
        auto CTD = dyn_cast<ClassTemplateDecl>(TD);
        if (CTD->isThisDeclarationADefinition() == false)
          return;
      } else if (TD->getKind() == Decl::Kind::FunctionTemplate) {
        auto FTD = dyn_cast<FunctionTemplateDecl>(TD);
        if (FTD->isThisDeclarationADefinition() == false)
          return;
      }
      if (TD->getKind() == Decl::Kind::ClassTemplate) {
        auto CTD = dyn_cast<ClassTemplateDecl>(TD);
        auto CT = CTD->getTemplatedDecl();
        if (CT->isCompleteDefinition() == false)
          return;
      } else if (TD->getKind() == Decl::Kind::FunctionTemplate) {
        auto FTD = dyn_cast<FunctionTemplateDecl>(TD);
        auto FT = FTD->getTemplatedDecl();
        if (FT->isDefined() == false)
          return;
      }
      if (TD->getKind() == Decl::Kind::ClassTemplate) {
        auto CTD = dyn_cast<ClassTemplateDecl>(TD);
        auto CT = CTD->getTemplatedDecl();
        if (CT->isCompleteDefinition() == false)
          return;
      } else if (TD->getKind() == Decl::Kind::FunctionTemplate) {
        auto FTD = dyn_cast<FunctionTemplateDecl>(TD);
        auto FT = FTD->getTemplatedDecl();
        if (FT->isDefined() == false)
          return;
      }
      if (TD->getKind() == Decl::Kind::ClassTemplate) {
        auto CTD = dyn_cast<ClassTemplateDecl>(TD);
        auto CT = CTD->getTemplatedDecl();
        if (CT->isCompleteDefinition() == false)
          return;
      } else if (TD->getKind() == Decl::Kind::FunctionTemplate) {
        auto FTD = dyn_cast<FunctionTemplateDecl>(TD);
        auto FT = FTD->getTemplatedDecl();
        if (FT->isDefined() == false)
          return;
      }
      if (TD->getKind() == Decl::Kind::ClassTemplate) {
        auto CTD = dyn_cast<ClassTemplateDecl>(TD);
        auto CT = CTD->getTemplatedDecl();
        if (CT->isCompleteDefinition() == false)
          return;
      } else if (TD->getKind() == Decl::Kind::FunctionTemplate) {
        auto FTD = dyn_cast<FunctionTemplateDecl>(TD);
        auto FT = FTD->getTemplatedDecl();
        if (FT->isDefined() == false)
          return;
      }
      if (TD->getKind() == Decl::Kind::ClassTemplate) {
        auto CTD = dyn_cast<ClassTemplateDecl>(TD);
        auto CT = CTD->getTemplatedDecl();
        if (CT->isCompleteDefinition() == false)
          return;
      } else if (TD->getKind() == Decl::Kind::FunctionTemplate) {
        auto FTD = dyn_cast<FunctionTemplateDecl>(TD);
        auto FT = FTD->getTemplatedDecl();
        if (FT->isDefined() == false)
          return;
      }
      if (TD->getKind() == Decl::Kind::ClassTemplate) {
        auto CTD = dyn_cast<ClassTemplateDecl>(TD);
        auto CT = CTD->getTemplatedDecl();
        if (CT->isCompleteDefinition() == false)
          return;
      } else if (TD->getKind() == Decl::Kind::FunctionTemplate) {
        auto FTD = dyn_cast<FunctionTemplateDecl>(TD);
        auto FT = FTD->getTemplatedDecl();
        if (FT->isDefined() == false)
          return;
      }
      if (TD->getKind() == Decl::Kind::ClassTemplate) {
        auto CTD = dyn_cast<ClassTemplateDecl>(TD);
        auto CT = CTD->getTemplatedDecl();
        if (CT->isCompleteDefinition() == false)
          return;
      } else if (TD->getKind() == Decl::Kind::FunctionTemplate) {
        auto FTD = dyn_cast<FunctionTemplateDecl>(TD);
        auto FT = FTD->getTemplatedDecl();
        if (FT->isDefined() == false)
          return;
      }
      if (TD->getKind() == Decl::Kind::ClassTemplate) {
        auto CTD = dyn_cast<ClassTemplateDecl>(TD);
        auto CT = CTD->getTemplatedDecl();
        if (CT->isCompleteDefinition() == false)
          return;
      } else if (TD->getKind() == Decl::Kind::FunctionTemplate) {
        auto FTD = dyn_cast<FunctionTemplateDecl>(TD);
        auto FT = FTD->getTemplatedDecl();
        if (FT->isDefined() == false)
          return;
      }
      if (TD->getKind() == Decl::Kind::ClassTemplate) {
        auto CTD = dyn_cast<ClassTemplateDecl>(TD);
        auto CT = CTD->getTemplatedDecl();
        if (CT->isCompleteDefinition() == false)
          return;
      } else if (TD->getKind() == Decl::Kind::FunctionTemplate) {
        auto FTD = dyn_cast<FunctionTemplateDecl>(TD);
        auto FT = FTD->getTemplatedDecl();
        if (FT->isDefined() == false)
          return;
      }
      if (TD->getKind() == Decl::Kind::ClassTemplate) {
        auto CTD = dyn_cast<ClassTemplateDecl>(TD);
        auto CT = CTD->getTemplatedDecl();
        if (CT->isCompleteDefinition() == false)
          return;
      } else if (TD->getKind() == Decl::Kind::FunctionTemplate) {
        auto FTD = dyn_cast<FunctionTemplateDecl>(TD);
        auto FT = FTD->getTemplatedDecl();
        if (FT->isDefined() == false)
          return;
      }
      if (TD->getKind() == Decl::Kind::ClassTemplate) {
        auto CTD = dyn_cast<ClassTemplateDecl>(TD);
        auto CT = CTD->getTemplatedDecl();
        if (CT->isCompleteDefinition() == false)
          return;
      } else if (TD->getKind() == Decl::Kind::FunctionTemplate) {
        auto FTD = dyn_cast<FunctionTemplateDecl>(TD);
        auto FT = FTD->getTemplatedDecl();
        if (FT->isDefined() == false)
          return;
      }
      if (TD->getKind() == Decl::Kind::ClassTemplate) {
        auto CTD = dyn_cast<ClassTemplateDecl>(TD);
        auto CT = CTD->getTemplatedDecl();
        if (CT->isCompleteDefinition() == false)
          return;
      } else if (TD->getKind() == Decl::Kind::FunctionTemplate) {
        auto FTD = dyn_cast<FunctionTemplateDecl>(TD);
        auto FT = FTD->getTemplatedDecl();
        if (FT->isDefined() == false)
          return;
      }
      if (TD->getKind() == Decl::Kind::ClassTemplate) {
        auto CTD = dyn_cast<ClassTemplateDecl>(TD);
        auto CT = CTD->getTemplatedDecl();
        if (CT->isCompleteDefinition() == false)
          return;
      } else if (TD->getKind() == Decl::Kind::FunctionTemplate) {
        auto FTD = dyn_cast<FunctionTemplateDecl>(TD);
        auto FT = FTD->getTemplatedDecl();
        if (FT->isDefined() == false)
          return;
      }
      if (TD->getKind() == Decl::Kind::ClassTemplate) {
        auto CTD = dyn_cast<ClassTemplateDecl>(TD);
        auto CT = CTD->getTemplatedDecl();
        if (CT->isCompleteDefinition() == false)
          return;
      } else if (TD->getKind() == Decl::Kind::FunctionTemplate) {
        auto FTD = dyn_cast<FunctionTemplateDecl>(TD);
        auto FT = FTD->getTemplatedDecl();
        if (FT->isDefined() == false)
          return;
      }
      if (TD->getKind() == Decl::Kind::ClassTemplate) {
        auto CTD = dyn_cast<ClassTemplateDecl>(TD);
        auto CT = CTD->getTemplatedDecl();
        if (CT->isCompleteDefinition() == false)
          return;
      } else if (TD->getKind() == Decl::Kind::FunctionTemplate) {
        auto FTD = dyn_cast<FunctionTemplateDecl>(TD);
        auto FT = FTD->getTemplatedDecl();
        if (FT->isDefined() == false)
          return;
      }
      if (TD->getKind() == Decl::Kind::ClassTemplate) {
        auto CTD = dyn_cast<ClassTemplateDecl>(TD);
        auto CT = CTD->getTemplatedDecl();
        if (CT->isCompleteDefinition() == false)
          return;
      } else if (TD->getKind() == Decl::Kind::FunctionTemplate) {
        auto FTD = dyn_cast<FunctionTemplateDecl>(TD);
        auto FT = FTD->getTemplatedDecl();
        if (FT->isDefined() == false)
          return;
      }
      if (TD->getKind() == Decl::Kind::ClassTemplate) {
        auto CTD = dyn_cast<ClassTemplateDecl>(TD);
        auto CT = CTD->getTemplatedDecl();
        if (CT->isCompleteDefinition() == false)
          return;
      } else if (TD->getKind() == Decl::Kind::FunctionTemplate) {
        auto FTD = dyn_cast<FunctionTemplateDecl>(TD);
        auto FT = FTD->getTemplatedDecl();
        if (FT->isDefined() == false)
          return;
      }
      if (TD->getKind() == Decl::Kind::ClassTemplate) {
        auto CTD = dyn_cast<ClassTemplateDecl>(TD);
        auto CT = CTD->getTemplatedDecl();
        if (CT->isCompleteDefinition() == false)
          return;
      } else if (TD->getKind() == Decl::Kind::FunctionTemplate) {
        auto FTD = dyn_cast<FunctionTemplateDecl>(TD);
        auto FT = FTD->getTemplatedDecl();
        if (FT->isDefined() == false)
          return;
      }
      if (TD->getKind() == Decl::Kind::ClassTemplate) {
        auto CTD = dyn_cast<ClassTemplateDecl>(TD);
        auto CT = CTD->getTemplatedDecl();
        if (CT->isCompleteDefinition() == false)
          return;
      } else if (TD->getKind() == Decl::Kind::FunctionTemplate) {
        auto FTD = dyn_cast<FunctionTemplateDecl>(TD);
        auto FT = FTD->getTemplatedDecl();
        if (FT->isDefined() == false)
          return;
      }
      if (TD->getKind() == Decl::Kind::ClassTemplate) {
        auto CTD = dyn_cast<ClassTemplateDecl>(TD);
        auto CT = CTD->getTemplatedDecl();
        if (CT->isCompleteDefinition() == false)
          return;
      } else if (TD->getKind() == Decl::Kind::FunctionTemplate) {
        auto FTD = dyn_cast<FunctionTemplateDecl>(TD);
        auto FT = FTD->getTemplatedDecl();
        if (FT->isDefined() == false)
          return;
      }
      if (TD->getKind() == Decl::Kind::ClassTemplate) {
        auto CTD = dyn_cast<ClassTemplateDecl>(TD);
        auto CT = CTD->getTemplatedDecl();
        if (CT->isCompleteDefinition() == false)
          return;
      } else if (TD->getKind() == Decl::Kind::FunctionTemplate) {
        auto FTD = dyn_cast<FunctionTemplateDecl>(TD);
        auto FT = FTD->getTemplatedDecl();
        if (FT->isDefined() == false)
          return;
      }
      if (TD->getKind() == Decl::Kind::ClassTemplate) {
        auto CTD = dyn_cast<ClassTemplateDecl>(TD);
        auto CT = CTD->getTemplatedDecl();
        if (CT->isCompleteDefinition() == false)
          return;
      } else if (TD->getKind() == Decl::Kind::FunctionTemplate) {
        auto FTD = dyn_cast<FunctionTemplateDecl>(TD);
        auto FT = FTD->getTemplatedDecl();
        if (FT->isDefined() == false)
          return;
      }
      if (TD->getKind() == Decl::Kind::ClassTemplate) {
        auto CTD = dyn_cast<ClassTemplateDecl>(TD);
        auto CT = CTD->getTemplatedDecl();
        if (CT->isCompleteDefinition() == false)
          return;
      } else if (TD->getKind() == Decl::Kind::FunctionTemplate) {
        auto FTD = dyn_cast<FunctionTemplateDecl>(TD);
        auto FT = FTD->getTemplatedDecl();
        if (FT->isDefined() == false)
          return;
      }
      if (TD->getKind() == Decl::Kind::ClassTemplate) {
        auto CTD = dyn_cast<ClassTemplateDecl>(TD);
        auto CT = CTD->getTemplatedDecl();
        if (CT->isCompleteDefinition() == false)
          return;
      } else if (TD->getKind() == Decl::Kind::FunctionTemplate) {
        auto FTD = dyn_cast<FunctionTemplateDecl>(TD);
        auto FT = FTD->getTemplatedDecl();
        if (FT->isDefined() == false)
          return;
      }
      if (TD->getKind() == Decl::Kind::ClassTemplate) {
        auto CTD = dyn_cast<ClassTemplateDecl>(TD);
        auto CT = CTD->getTemplatedDecl();
        if (CT->isCompleteDefinition() == false)
          return;
      } else if (TD->getKind() == Decl::Kind::FunctionTemplate) {
        auto FTD = dyn_cast<FunctionTemplateDecl>(TD);
        auto FT = FTD->getTemplatedDecl();
        if (FT->isDefined() == false)
          return;
      }
      if (TD->getKind() == Decl::Kind::ClassTemplate) {
        auto CTD = dyn_cast<ClassTemplateDecl>(TD);
        auto CT = CTD->getTemplatedDecl();
        if (CT->isCompleteDefinition() == false)
          return;
      } else if (TD->getKind() == Decl::Kind::FunctionTemplate) {
        auto FTD = dyn_cast<FunctionTemplateDecl>(TD);
        auto FT = FTD->getTemplatedDecl();
        if (FT->isDefined() == false)
          return;
      }
      if (TD->getKind() == Decl::Kind::ClassTemplate) {
        auto CTD = dyn_cast<ClassTemplateDecl>(TD);
        auto CT = CTD->getTemplatedDecl();
        if (CT->isCompleteDefinition() == false)
          return;
      } else if (TD->getKind() == Decl::Kind::FunctionTemplate) {
        auto FTD = dyn_cast<FunctionTemplateDecl>(TD);
        auto FT = FTD->getTemplatedDecl();
        if (FT->isDefined() == false)
          return;
      }
      if (TD->getKind() == Decl::Kind::ClassTemplate) {
        auto CTD = dyn_cast<ClassTemplateDecl>(TD);
        auto CT = CTD->getTemplatedDecl();
        if (CT->isCompleteDefinition() == false)
          return;
      } else if (TD->getKind() == Decl::Kind::FunctionTemplate) {
        auto FTD = dyn_cast<FunctionTemplateDecl>(TD);
        auto FT = FTD->getTemplatedDecl();
        if (FT->isDefined() == false)
          return;
      }
      if (TD->getKind() == Decl::Kind::ClassTemplate) {
        auto CTD = dyn_cast<ClassTemplateDecl>(TD);
        auto CT = CTD->getTemplatedDecl();
        if (CT->isCompleteDefinition() == false)
          return;
      } else if (TD->getKind() == Decl::Kind::FunctionTemplate) {
        auto FTD = dyn_cast<FunctionTemplateDecl>(TD);
        auto FT = FTD->getTemplatedDecl();
        if (FT->isDefined() == false)
          return;
      }
      if (TD->getKind() == Decl::Kind::ClassTemplate) {
        auto CTD = dyn_cast<ClassTemplateDecl>(TD);
        auto CT = CTD->getTemplatedDecl();
        if (CT->isCompleteDefinition() == false)
          return;
      } else if (TD->getKind() == Decl::Kind::FunctionTemplate) {
        auto FTD = dyn_cast<FunctionTemplateDecl>(TD);
        auto FT = FTD->getTemplatedDecl();
        if (FT->isDefined() == false)
          return;
      }
      if (TD->getKind() == Decl::Kind::ClassTemplate) {
        auto CTD = dyn_cast<ClassTemplateDecl>(TD);
        auto CT = CTD->getTemplatedDecl();
        if (CT->isCompleteDefinition() == false)
          return;
      } else if (TD->getKind() == Decl::Kind::FunctionTemplate) {
        auto FTD = dyn_cast<FunctionTemplateDecl>(TD);
        auto FT = FTD->getTemplatedDecl();
        if (FT->isDefined() == false)
          return;
      }
      if (TD->getKind() == Decl::Kind::ClassTemplate) {
        auto CTD = dyn_cast<ClassTemplateDecl>(TD);
        auto CT = CTD->getTemplatedDecl();
        if (CT->isCompleteDefinition() == false)
          return;
      } else if (TD->getKind() == Decl::Kind::FunctionTemplate) {
        auto FTD = dyn_cast<FunctionTemplateDecl>(TD);
        auto FT = FTD->getTemplatedDecl();
        if (FT->isDefined() == false)
          return;
      }
      if (TD->getKind() == Decl::Kind::ClassTemplate) {
        auto CTD = dyn_cast<ClassTemplateDecl>(TD);
        auto CT = CTD->getTemplatedDecl();
        if (CT->isCompleteDefinition() == false)
          return;
      } else if (TD->getKind() == Decl::Kind::FunctionTemplate) {
        auto FTD = dyn_cast<FunctionTemplateDecl>(TD);
        auto FT = FTD->getTemplatedDecl();
        if (FT->isDefined() == false)
          return;
      }
      if (TD->getKind() == Decl::Kind::ClassTemplate) {
        auto CTD = dyn_cast<ClassTemplateDecl>(TD);
        auto CT = CTD->getTemplatedDecl();
        if (CT->isCompleteDefinition() == false)
          return;
      } else if (TD->getKind() == Decl::Kind::FunctionTemplate) {
        auto FTD = dyn_cast<FunctionTemplateDecl>(TD);
        auto FT = FTD->getTemplatedDecl();
        if (FT->isDefined() == false)
          return;
      }
      if (TD->getKind() == Decl::Kind::ClassTemplate) {
        auto CTD = dyn_cast<ClassTemplateDecl>(TD);
        auto CT = CTD->getTemplatedDecl();
        if (CT->isCompleteDefinition() == false)
          return;
      } else if (TD->getKind() == Decl::Kind::FunctionTemplate) {
        auto FTD = dyn_cast<FunctionTemplateDecl>(TD);
        auto FT = FTD->getTemplatedDecl();
        if (FT->isDefined() == false)
          return;
      }
      if (TD->getKind() == Decl::Kind::ClassTemplate) {
        auto CTD = dyn_cast<ClassTemplateDecl>(TD);
        auto CT = CTD->getTemplatedDecl();
        if (CT->isCompleteDefinition() == false)
          return;
      } else if (TD->getKind() == Decl::Kind::FunctionTemplate) {
        auto FTD = dyn_cast<FunctionTemplateDecl>(TD);
        auto FT = FTD->getTemplatedDecl();
        if (FT->isDefined() == false)
          return;
      }
      if (TD->getKind() == Decl::Kind::ClassTemplate) {
        auto CTD = dyn_cast<ClassTemplateDecl>(TD);
        auto CT = CTD->getTemplatedDecl();
        if (CT->isCompleteDefinition() == false)
          return;
      } else if (TD->getKind() == Decl::Kind::FunctionTemplate) {
        auto FTD = dyn_cast<FunctionTemplateDecl>(TD);
        auto FT = FTD->getTemplatedDecl();
        if (FT->isDefined() == false)
          return;
      }
      if (TD->getKind() == Decl::Kind::ClassTemplate) {
        auto CTD = dyn_cast<ClassTemplateDecl>(TD);
        auto CT = CTD->getTemplatedDecl();
        if (CT->isCompleteDefinition() == false)
          return;
      } else if (TD->getKind() == Decl::Kind::FunctionTemplate) {
        auto FTD = dyn_cast<FunctionTemplateDecl>(TD);
        auto FT = FTD->getTemplatedDecl();
        if (FT->isDefined() == false)
          return;
      }
      if (TD->getKind() == Decl::Kind::ClassTemplate) {
        auto CTD = dyn_cast<ClassTemplateDecl>(TD);
        auto CT = CTD->getTemplatedDecl();
        if (CT->isCompleteDefinition() == false)
          return;
      } else if (TD->getKind() == Decl::Kind::FunctionTemplate) {
        auto FTD = dyn_cast<FunctionTemplateDecl>(TD);
        auto FT = FTD->getTemplatedDecl();
        if (FT->isDefined() == false)
          return;
      }
      if (TD->getKind() == Decl::Kind::ClassTemplate) {
        auto CTD = dyn_cast<ClassTemplateDecl>(TD);
        auto CT = CTD->getTemplatedDecl();
        if (CT->isCompleteDefinition() == false)
          return;
      } else if (TD->getKind() == Decl::Kind::FunctionTemplate) {
        auto FTD = dyn_cast<FunctionTemplateDecl>(TD);
        auto FT = FTD->getTemplatedDecl();
        if (FT->isDefined() == false)
          return;
      }
      if (TD->getKind() == Decl::Kind::ClassTemplate) {
        auto CTD = dyn_cast<ClassTemplateDecl>(TD);
        auto CT = CTD->getTemplatedDecl();
        if (CT->isCompleteDefinition() == false)
          return;
      } else if (TD->getKind() == Decl::Kind::FunctionTemplate) {
        auto FTD = dyn_cast<FunctionTemplateDecl>(TD);
        auto FT = FTD->getTemplatedDecl();
        if (FT->isDefined() == false)
          return;
      }
      if (TD->getKind() == Decl::Kind::ClassTemplate) {
        auto CTD = dyn_cast<ClassTemplateDecl>(TD);
        auto CT = CTD->getTemplatedDecl();
        if (CT->isCompleteDefinition() == false)
          return;
      } else if (TD->getKind() == Decl::Kind::FunctionTemplate) {
        auto FTD = dyn_cast<FunctionTemplateDecl>(TD);
        auto FT = FTD->getTemplatedDecl();
        if (FT->isDefined() == false)
          return;
      }
      if (TD->getKind() == Decl::Kind::ClassTemplate) {
        auto CTD = dyn_cast<ClassTemplateDecl>(TD);
        auto CT = CTD->getTemplatedDecl();
        if (CT->isCompleteDefinition() == false)
          return;
      } else if (TD->getKind() == Decl::Kind::FunctionTemplate) {
        auto FTD = dyn_cast<FunctionTemplateDecl>(TD);
        auto FT = FTD->getTemplatedDecl();
        if (FT->isDefined() == false)
          return;
      }
      if (TD->getKind() == Decl::Kind::ClassTemplate) {
        auto CTD = dyn_cast<ClassTemplateDecl>(TD);
        auto CT = CTD->getTemplatedDecl();
        if (CT->isCompleteDefinition() == false)
          return;
      } else if (TD->getKind() == Decl::Kind::FunctionTemplate) {
        auto FTD = dyn_cast<FunctionTemplateDecl>(TD);
        auto FT = FTD->getTemplatedDecl();
        if (FT->isDefined() == false)
          return;
      }
      if (TD->getKind() == Decl::Kind::ClassTemplate) {
        auto CTD = dyn_cast<ClassTemplateDecl>(TD);
        auto CT = CTD->getTemplatedDecl();
        if (CT->isCompleteDefinition() == false)
          return;
      } else if (TD->getKind() == Decl::Kind::FunctionTemplate) {
        auto FTD = dyn_cast<FunctionTemplateDecl>(TD);
        auto FT = FTD->getTemplatedDecl();
        if (FT->isDefined() == false)
          return;
      }
      if (TD->getKind() == Decl::Kind::ClassTemplate) {
        auto CTD = dyn_cast<ClassTemplateDecl>(TD);
        auto CT = CTD->getTemplatedDecl();
        if (CT->isCompleteDefinition() == false)
          return;
      } else if (TD->getKind() == Decl::Kind::FunctionTemplate) {
        auto FTD = dyn_cast<FunctionTemplateDecl>(TD);
        auto FT = FTD->getTemplatedDecl();
        if (FT->isDefined() == false)
          return;
      }
      if (TD->getKind() == Decl::Kind::ClassTemplate) {
        auto CTD = dyn_cast<ClassTemplateDecl>(TD);
        auto CT = CTD->getTemplatedDecl();
        if (CT->isCompleteDefinition() == false)
          return;
      } else if (TD->getKind() == Decl::Kind::FunctionTemplate) {
        auto FTD = dyn_cast<FunctionTemplateDecl>(TD);
        auto FT = FTD->getTemplatedDecl();
        if (FT->isDefined() == false)
          return;
      }
      if (TD->getKind() == Decl::Kind::ClassTemplate) {
        auto CTD = dyn_cast<ClassTemplateDecl>(TD);
        auto CT = CTD->getTemplatedDecl();
        if (CT->isCompleteDefinition() == false)
          return;
      } else if (TD->getKind() == Decl::Kind::FunctionTemplate) {
        auto FTD = dyn_cast<FunctionTemplateDecl>(TD);
        auto FT = FTD->getTemplatedDecl();
        if (FT->isDefined() == false)
          return;
      }
      if (TD->getKind() == Decl::Kind::ClassTemplate) {
        auto CTD = dyn_cast<ClassTemplateDecl>(TD);
        auto CT = CTD->getTemplatedDecl();
        if (CT->isCompleteDefinition() == false)
          return;
      } else if (TD->getKind() == Decl::Kind::FunctionTemplate) {
        auto FTD = dyn_cast<FunctionTemplateDecl>(TD);
        auto FT = FTD->getTemplatedDecl();
        if (FT->isDefined() == false)
          return;
      }
      if (TD->getKind() == Decl::Kind::ClassTemplate) {
        auto CTD = dyn_cast<ClassTemplateDecl>(TD);
        auto CT = CTD->getTemplatedDecl();
        if (CT->isCompleteDefinition() == false)
          return;
      } else if (TD->getKind() == Decl::Kind::FunctionTemplate) {
        auto FTD = dyn_cast<FunctionTemplateDecl>(TD);
        auto FT = FTD->getTemplatedDecl();
        if (FT->isDefined() == false)
          return;
      }
      if (TD->getKind() == Decl::Kind::ClassTemplate) {
        auto CTD = dyn_cast<ClassTemplateDecl>(TD);
        auto CT = CTD->getTemplatedDecl();
        if (CT->isCompleteDefinition() == false)
          return;
      } else if (TD->getKind() == Decl::Kind::FunctionTemplate) {
        auto FTD = dyn_cast<FunctionTemplateDecl>(TD);
        auto FT = FTD->getTemplatedDecl();
        if (FT->isDefined() == false)
          return;
      }
      if (TD->getKind() == Decl::Kind::ClassTemplate) {
        auto CTD = dyn_cast<ClassTemplateDecl>(TD);
        auto CT = CTD->getTemplatedDecl();
        if (CT->isCompleteDefinition() == false)
          return;
      } else if (TD->getKind() == Decl::Kind::FunctionTemplate) {
        auto FTD = dyn_cast<FunctionTemplateDecl>(TD);
        auto FT = FTD->getTemplatedDecl();
        if (FT->isDefined() == false)
          return;
      }
      if (TD->getKind() == Decl::Kind::ClassTemplate) {
        auto CTD = dyn_cast<ClassTemplateDecl>(TD);
        auto CT = CTD->getTemplatedDecl();
        if (CT->isCompleteDefinition() == false)
          return;
      } else if (TD->getKind() == Decl::Kind::FunctionTemplate) {
        auto FTD = dyn_cast<FunctionTemplateDecl>(TD);
        auto FT = FTD->getTemplatedDecl();
        if (FT->isDefined() == false)
          return;
      }
      if (TD->getKind() == Decl::Kind::ClassTemplate) {
        auto CTD = dyn_cast<ClassTemplateDecl>(TD);
        auto CT = CTD->getTemplatedDecl();
        if (CT->isCompleteDefinition() == false)
          return;
      } else if (TD->getKind() == Decl::Kind::FunctionTemplate) {
        auto FTD = dyn_cast<FunctionTemplateDecl>(TD);
        auto FT = FTD->getTemplatedDecl();
        if (FT->isDefined() == false)
          return;
      }
      if (TD->getKind() == Decl::Kind::ClassTemplate) {
        auto CTD = dyn_cast<ClassTemplateDecl>(TD);
        auto CT = CTD->getTemplatedDecl();
        if (CT->isCompleteDefinition() == false)
          return;
      } else if (TD->getKind() == Decl::Kind::FunctionTemplate) {
        auto FTD = dyn_cast<FunctionTemplateDecl>(TD);
        auto FT = FTD->getTemplatedDecl();
        if (FT->isDefined() == false)
          return;
      }
      if (TD->getKind() == Decl::Kind::ClassTemplate) {
        auto CTD = dyn_cast<ClassTemplateDecl>(TD);
        auto CT = CTD->getTemplatedDecl();
        if (CT->isCompleteDefinition() == false)
          return;
      } else if (TD->getKind() == Decl::Kind::FunctionTemplate) {
        auto FTD = dyn_cast<FunctionTemplateDecl>(TD);
        auto FT = FTD->getTemplatedDecl();
        if (FT->isDefined() == false)
          return;
      }
      if (TD->getKind() == Decl::Kind::ClassTemplate) {
        auto CTD = dyn_cast<ClassTemplateDecl>(TD);
        auto CT = CTD->getTemplatedDecl();
        if (CT->isCompleteDefinition() == false)
          return;
      } else if (TD->getKind() == Decl::Kind::FunctionTemplate) {
        auto FTD = dyn_cast<FunctionTemplateDecl>(TD);
        auto FT = FTD->getTemplatedDecl();
        if (FT->isDefined() == false)
          return;
      }
      if (TD->getKind() == Decl::Kind::ClassTemplate) {
        auto CTD = dyn_cast<ClassTemplateDecl>(TD);
        auto CT = CTD->getTemplatedDecl();
        if (CT->isCompleteDefinition() == false)
          return;
      } else if (TD->getKind() == Decl::Kind::FunctionTemplate) {
        auto FTD = dyn_cast<FunctionTemplateDecl>(TD);
        auto FT = FTD->getTemplatedDecl();
        if (FT->isDefined() == false)
          return;
      }
      if (TD->getKind() == Decl::Kind::ClassTemplate) {
        auto CTD = dyn_cast<ClassTemplateDecl>(TD);
        auto CT = CTD->getTemplatedDecl();
        if (CT->isCompleteDefinition() == false)
          return;
      } else if (TD->getKind() == Decl::Kind::FunctionTemplate) {
        auto FTD = dyn_cast<FunctionTemplateDecl>(TD);
        auto FT = FTD->getTemplatedDecl();
        if (FT->isDefined() == false)
          return;
      }
      if (TD->getKind() == Decl::Kind::ClassTemplate) {
        auto CTD = dyn_cast<ClassTemplateDecl>(TD);
        auto CT = CTD->getTemplatedDecl();
        if (CT->isCompleteDefinition() == false)
          return;
      } else if (TD->getKind() == Decl::Kind::FunctionTemplate) {
        auto FTD = dyn_cast<FunctionTemplateDecl>(TD);
        auto FT = FTD->getTemplatedDecl();
        if (FT->isDefined() == false)
          return;
      }
      if (TD->getKind() == Decl::Kind::ClassTemplate) {
        auto CTD = dyn_cast<ClassTemplateDecl>(TD);
        auto CT = CTD->getTemplatedDecl();
        if (CT->isCompleteDefinition() == false)
          return;
      } else if (TD->getKind() == Decl::Kind::FunctionTemplate) {
        auto FTD = dyn_cast<FunctionTemplateDecl>(TD);
        auto FT = FTD->getTemplatedDecl();
        if (FT->isDefined() == false)
          return;
      }
      if (TD->getKind() == Decl::Kind::ClassTemplate) {
        auto