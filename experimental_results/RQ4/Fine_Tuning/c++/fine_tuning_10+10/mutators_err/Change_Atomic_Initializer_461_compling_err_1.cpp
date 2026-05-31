//header file
#pragma once
#include "Mutator_base.h"

/**
 * Change_Atomic_Initializer_461
 */ 
class MutatorFrontendAction_461 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(461)

private:
    class MutatorASTConsumer_461 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_461(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/change_atomic_initializer_461.h"

// ========================================================================================================
#define MUT461_OUTPUT 1

void MutatorFrontendAction_461::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("atomicVar")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      
      if (VD->hasInit()) {
        //Get the source code text of target node
        auto *InitExpr = VD->getInit();
        if (auto *IntLit = llvm::dyn_cast<clang::IntegerLiteral>(InitExpr)) {
          //Perform mutation on the source code text by applying string replacement
          auto InitType = VD->getType();
          if (InitType->isPointerType()) {
            std::string MutatedInitializer = "(int *)1";
            //Replace the original AST node with the mutated one
            Rewrite.ReplaceText(IntLit->getSourceRange(), MutatedInitializer);
          }
        }
      }
    }
}
  
void MutatorFrontendAction_461::MutatorASTConsumer_461::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(hasType(isAtomicType())).bind("atomicVar");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}