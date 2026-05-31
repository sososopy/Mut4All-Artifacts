//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Return_Type_With_Pack_Indexing_485
 */ 
class MutatorFrontendAction_485 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(485)

private:
    class MutatorASTConsumer_485 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_485(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_.h"

// ========================================================================================================
#define MUT485_OUTPUT 1

void MutatorFrontendAction_485::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Check if it's a function template with at least one template parameter pack
      auto *FT = FD->getDescribedFunctionTemplate();
      if (!FT) return;
      bool hasPack = false;
      std::string packName;
      for (auto *Param : FT->getTemplateParameters()) {
        if (Param->isTemplateParameterPack()) {
          hasPack = true;
          packName = Param->getNameAsString();
          break;
        }
      }
      if (!hasPack) return;
      //Check if return type is already a pack indexing type
      auto ReturnType = FD->getReturnType();
      if (ReturnType->getTypeClass() == clang::Type::PackIndexing) return;
      //Get the source code text of target node
      auto ReturnTypeSourceRange = FD->getReturnTypeSourceRange();
      if (ReturnTypeSourceRange.isInvalid()) return;
      std::string ReturnTypeText = stringutils::rangetoStr(*(Result.SourceManager), ReturnTypeSourceRange);
      //Perform mutation on the source code text by applying string replacement
      std::string NewReturnType = packName + "...[0]";
      //Adjust for reference types
      if (ReturnType->isReferenceType()) {
        if (ReturnType->isLValueReferenceType()) NewReturnType += "&";
        else if (ReturnType->isRValueReferenceType()) NewReturnType += "&&";
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(ReturnTypeSourceRange, NewReturnType);
    }
}
  
void MutatorFrontendAction_485::MutatorASTConsumer_485::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(hasDescendant(functionTemplateDecl())).bind("FunctionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}