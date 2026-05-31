//header file
#pragma once
#include "Mutator_base.h"

/**
 * Mutate_Paren_Init_Of_Array_142
 */ 
class MutatorFrontendAction_142 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(142)

private:
    class MutatorASTConsumer_142 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_142(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutate_Paren_Init_Of_Array_142.h"

// ========================================================================================================
#define MUT142_OUTPUT 1

void MutatorFrontendAction_142::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::InitListExpr>("ParenArray")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;

      auto DL = Result.Nodes.getNodeAs<clang::VarDecl>("ArrayDecl");
      if (!DL || !DL->getType()->isArrayType())
        return;
      //Get the source code text of target node
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), MT->getSourceRange());
      auto array_type = DL->getType()->getAsArrayTypeUnsafe();
      if (auto *CAT = dyn_cast<ConstantArrayType>(array_type)) {
        unsigned int sz = CAT->getSize().getZExtValue();
        llvm::outs() << "Array size: " << sz << '\n';
        if (sz == 0) {
          content = "()";
        } else {
          content = "(";
          for (unsigned int i = 0; i < sz; ++i) {
            if (i > 0)
              content += ',';
            content += getrandom::getRandomIndex(100);
          }
          content += ')';
        }
      } else if (auto *VAT = dyn_cast<VariableArrayType>(array_type)) {
        unsigned int sz = getrandom::getRandomIndex(5) + 1;
        llvm::outs() << "Array size: " << sz << '\n';
        content = "(";
        for (unsigned int i = 0; i < sz; ++i) {
          if (i > 0)
            content += ',';
          content += getrandom::getRandomIndex(100);
        }
        content += ')';
      } else if (auto *IVAT = dyn_cast<IncompleteArrayType>(array_type)) {
        unsigned int sz = getrandom::getRandomIndex(5) + 1;
        llvm::outs() << "Array size: " << sz << '\n';
        content = "(";
        for (unsigned int i = 0; i < sz; ++i) {
          if (i > 0)
            content += ',';
          content += getrandom::getRandomIndex(100);
        }
        content += ')';
      } else {
        return;
      }

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_142::MutatorASTConsumer_142::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = initListExpr(hasParent(varDecl(hasType(arrayType()))),
                                isParenInit())
                       .bind("ParenArray");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}