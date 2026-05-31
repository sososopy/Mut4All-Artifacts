//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Alias_Instantiation_Without_Specifying_All_Template_Parameters_410
 */ 
class MutatorFrontendAction_410 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(410)

private:
    class MutatorASTConsumer_410 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_410(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Template_Alias_Instantiation_Without_Specifying_All_Template_Parameters_410.h"

// ========================================================================================================
#define MUT410_OUTPUT 1

void MutatorFrontendAction_410::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TAD = Result.Nodes.getNodeAs<clang::TypeAliasDecl>("TypeAlias")) {
      if (!TAD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TAD->getLocation()))
        return;
      
      if (!TAD->isTemplated())
        return;
      
      if (!TAD->getDescribedAliasTemplate())
        return;
      
      auto TPL = TAD->getDescribedAliasTemplate();
      if (!TPL->isThisDeclarationADefinition())
        return;
      
      auto TPL_parms = TPL->getTemplateParameters();
      if (TPL_parms->size() <= 1)
        return;
      
      bool has_default = false;
      for (auto parm : *TPL_parms) {
        if (parm->hasDefaultArgument()) {
          has_default = true;
          break;
        }
      }
      
      if (!has_default)
        return;
      
      auto alias_name = TAD->getNameAsString();
      string ins = alias_name + "<";
      for (auto parm : *TPL_parms) {
        if (!parm->hasDefaultArgument()) {
          ins += parm->getNameAsString() + ",";
        }
      }
      if (ins.back() == ',')
        ins.pop_back();
      ins += "> mut_410;";
      llvm::outs() << ins << '\n';
      Rewrite.ReplaceText(TAD->getEndLoc(), 0, "/*mut410*/" + ins);
    }
}
  
void MutatorFrontendAction_410::MutatorASTConsumer_410::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = typeAliasDecl().bind("TypeAlias");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}