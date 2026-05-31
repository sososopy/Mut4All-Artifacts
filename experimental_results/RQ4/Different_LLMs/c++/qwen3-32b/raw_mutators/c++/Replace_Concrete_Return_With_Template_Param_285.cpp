//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Concrete_Return_With_Template_Param_285
 */ 
class MutatorFrontendAction_285 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(285)

private:
    class MutatorASTConsumer_285 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_285(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Concrete_Return_With_Template_Param_285.h"

// ========================================================================================================
#define MUT285_OUTPUT 1

void MutatorFrontendAction_285::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("functionDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      FunctionTemplateDecl *FTD = FD->getDescribedFunctionTemplate();
      if (!FTD)
        return;

      const TemplateParameterList *TPL = FTD->getTemplateParameters();
      if (!TPL || TPL->size() < 1)
        return;

      for (const auto *Param : *TPL) {
        if (const auto *TTP = dyn_cast<TypeTemplateParameter>(Param)) {
          std::string typeName = TTP->getNameAsString();
          if (typeName.empty())
            continue;

          QualType returnType = FD->getReturnType();
          if (returnType->getAs<TemplateTypeParmType>())
            continue;

          TypeSourceInfo *TSI = FD->getReturnTypeSourceInfo();
          if (!TSI)
            continue;

          TypeLoc returnTypeLoc = TSI->getTypeLoc();
          SourceLocation startLoc = returnTypeLoc.getBeginLoc();
          SourceLocation endLoc = returnTypeLoc.getEndLoc();

          if (startLoc.isInvalid() || endLoc.isInvalid())
            continue;

          Rewrite.ReplaceText(SourceRange(startLoc, endLoc), typeName);
          break;
        }
      }
    }
}
  
void MutatorFrontendAction_285::MutatorASTConsumer_285::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(hasParent(functionTemplateDecl())).bind("functionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}