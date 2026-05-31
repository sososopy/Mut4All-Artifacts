//header file
#pragma once
#include "Mutator_base.h"

/**
 * Incomplete_Array_Union_Initialization_301
 */ 
class MutatorFrontendAction_301 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(301)

private:
    class MutatorASTConsumer_301 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_301(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/incomplete_array_union_initialization_301.h"

// ========================================================================================================
#define MUT301_OUTPUT 1

void MutatorFrontendAction_301::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *UD = Result.Nodes.getNodeAs<clang::UnionDecl>("UnionDecl")) {
      if (!UD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     UD->getLocation()))
        return;

      for (auto *Field : UD->fields()) {
        if (auto *AT = Field->getType()->getAsArrayTypeUnsafe()) {
          std::string unionName = UD->getNameAsString();
          std::string fieldType = Field->getType().getBaseTypeIdentifier()->getName().str();
          std::string mutatedUnion = "static union " + unionName + " { " + fieldType + " " + Field->getNameAsString() + "[]; } u = {2.5};";
          Rewrite.ReplaceText(UD->getSourceRange(), mutatedUnion);
          break;
        }
      }
    }
}
  
void MutatorFrontendAction_301::MutatorASTConsumer_301::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = unionDecl(has(fieldDecl(hasType(arrayType())))).bind("UnionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}