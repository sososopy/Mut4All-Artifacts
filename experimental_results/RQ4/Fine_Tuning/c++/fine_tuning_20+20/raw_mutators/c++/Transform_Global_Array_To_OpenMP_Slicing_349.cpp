//header file
#pragma once
#include "Mutator_base.h"

/**
 * Transform_Global_Array_To_OpenMP_Slicing_349
 */ 
class MutatorFrontendAction_349 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(349)

private:
    class MutatorASTConsumer_349 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_349(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Transform_Global_Array_To_OpenMP_Slicing_349.h"

// ========================================================================================================
#define MUT349_OUTPUT 1

void MutatorFrontendAction_349::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("GlobalArray")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      //Get the source code text of target node
      auto type = VD->getType();
      if (!type->isArrayType())
        return;
      auto DL = type->getAsArrayTypeUnsafe();
      if (DL == nullptr)
        return;
      auto arraytype = DL->getElementType();
      if (DL->getSizeExpr() != nullptr)
        return;
      auto name = VD->getNameAsString();
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), VD->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      string pragma = "#pragma omp task depend(inout: " + name + "[:][:])\n";
      content = pragma + content;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(VD->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_349::MutatorASTConsumer_349::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(hasGlobalStorage()).bind("GlobalArray");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}