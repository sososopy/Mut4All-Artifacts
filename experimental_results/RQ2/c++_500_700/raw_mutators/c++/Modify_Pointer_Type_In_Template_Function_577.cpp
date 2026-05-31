```cpp
//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_pointer_type_in_template_function_577
 */ 
class MutatorFrontendAction_577 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(577)

private:
    class MutatorASTConsumer_577 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_577(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_pointer_type_in_template_function_577.h"

// ========================================================================================================
#define MUT577_OUTPUT 1

void MutatorFrontendAction_577::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("TemplateFunction")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      //Get the source code text of target node
      for (auto *Spec : FD->specializations()) {
        if (Spec->getTemplatedDecl()->getNumParams() == 1) {
          auto *Param = Spec->getTemplatedDecl()->getParamDecl(0);
          if (auto *PT = llvm::dyn_cast<clang::PointerType>(Param->getType().getTypePtr())) {
            //Perform mutation on the source code text by applying string replacement
            std::string originalType = Param->getType().getAsString();
            std::string mutatedType = "volatile " + PT->getPointeeType().getAsString() + "*";
            //Replace the original AST node with the mutated one
            Rewrite.ReplaceText(Param->getSourceRange(), mutatedType);
          }
        }
      }
    }
}
  
void MutatorFrontendAction_577::MutatorASTConsumer_577::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionTemplateDecl(hasDescendant(parmVarDecl(hasType(pointerType())))).bind("TemplateFunction");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
```