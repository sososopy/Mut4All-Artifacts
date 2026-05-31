//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Diagnose_If_Condition_With_Unexpanded_Pack_383
 */ 
class MutatorFrontendAction_383 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(383)

private:
    class MutatorASTConsumer_383 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_383(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::FunctionDecl*> templateFunctions;
    };
};

//source file
#include "../include/Mutator_Replace_Diagnose_If_Condition_With_Unexpanded_Pack_383.h"

// ========================================================================================================
#define MUT383_OUTPUT 1

void MutatorFrontendAction_383::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("templateFunction")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Record template functions with parameter packs
      if (FD->isTemplateInstantiation() || !FD->getDescribedFunctionTemplate())
        return;
      auto *Template = FD->getDescribedFunctionTemplate();
      if (!Template->getTemplateParameters())
        return;
      bool hasPack = false;
      for (auto *Param : *Template->getTemplateParameters()) {
        if (Param->isTemplateParameterPack()) {
          hasPack = true;
          break;
        }
      }
      if (hasPack) {
        templateFunctions.push_back(FD);
      }
    }
    else if (auto *Attr = Result.Nodes.getNodeAs<clang::Attr>("diagnoseIfAttr")) {
      //Filter nodes in header files
      if (!Attr || !Result.Context->getSourceManager().isWrittenInMainFile(
                     Attr->getLocation()))
        return;
      //Get the source code text of target node
      auto *Decl = Result.Nodes.getNodeAs<clang::Decl>("attachedDecl");
      if (!Decl)
        return;
      //Check if attached declaration is inside a recorded template function
      bool insideTemplate = false;
      clang::FunctionDecl* enclosingTemplate = nullptr;
      for (auto *TF : templateFunctions) {
        if (Decl->getDeclContext() == TF || 
            (Decl->getDeclContext() && Decl->getDeclContext()->getParent() == TF)) {
          insideTemplate = true;
          enclosingTemplate = TF;
          break;
        }
      }
      if (!insideTemplate || !enclosingTemplate)
        return;
      //Get template parameter pack name
      auto *Template = enclosingTemplate->getDescribedFunctionTemplate();
      std::string packName;
      for (auto *Param : *Template->getTemplateParameters()) {
        if (Param->isTemplateParameterPack()) {
          packName = Param->getNameAsString();
          break;
        }
      }
      if (packName.empty())
        return;
      //Perform mutation on the source code text by applying string replacement
      std::string attrText = stringutils::rangetoStr(*(Result.SourceManager), Attr->getRange());
      //Find condition argument position
      size_t condStart = attrText.find('(');
      if (condStart == std::string::npos)
        return;
      size_t condEnd = attrText.find(',', condStart);
      if (condEnd == std::string::npos)
        condEnd = attrText.find(')', condStart);
      if (condEnd == std::string::npos)
        return;
      std::string newAttr = attrText.substr(0, condStart+1) + packName + attrText.substr(condEnd);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(Attr->getRange()), newAttr);
    }
}
  
void MutatorFrontendAction_383::MutatorASTConsumer_383::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher templateMatcher = functionDecl(isTemplateInstantiation()).bind("templateFunction");
    DeclarationMatcher attrMatcher = decl(hasAttr(clang::attr::DiagnoseIf)).bind("attachedDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(templateMatcher, &callback);
    matchFinder.addMatcher(attrMatcher, &callback);
    matchFinder.matchAST(Context);
}