//header file
#pragma once
#include "Mutator_base.h"

/**
 * Fold_Template_Parameter_Pack_Without_Operator_211
 */ 
class MutatorFrontendAction_211 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(211)

private:
    class MutatorASTConsumer_211 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_211(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Fold_Template_Parameter_Pack_Without_Operator_211.h"

// ========================================================================================================
#define MUT211_OUTPUT 1

void MutatorFrontendAction_211::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("TemplateFunctions")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      if (!MT->isThisDeclarationADefinition())
        return;
      auto parameters = MT->getTemplateParameters();
      bool has_pack = false;
      for (auto param : *parameters) {
        if (param->isTemplateParameterPack()) {
          has_pack = true;
          break;
        }
      }
      if (!has_pack)
        return;
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 MT->getSourceRange());
      llvm::outs() << declaration << '\n';
      auto pos = declaration.find("...");
      while (pos != string::npos) {
        declaration.insert(pos, "(");
        pos = declaration.find("...", pos + 5);
      }
      pos = declaration.find("...", 0);
      while (pos != string::npos) {
        declaration.insert(pos + 3, ")");
        pos = declaration.find("...", pos + 5);
      }
      llvm::outs() << declaration << '\n';
      int choice = getrandom::getRandomIndex(1);
      if (choice) {
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
      }
    }
}
  
void MutatorFrontendAction_211::MutatorASTConsumer_211::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl().bind("TemplateFunctions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}