//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_indirection_on_non_pointer_type_508
 */ 
class MutatorFrontendAction_508 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(508)

private:
    class MutatorASTConsumer_508 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_508(Rewriter &R) : TheRewriter(R) {}
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
        const VarDecl *existingNonPointerVar = nullptr;
    };
};

//source file
#include "../include/modify_indirection_on_non_pointer_type_508.h"

// ========================================================================================================
#define MUT508_OUTPUT 1

void MutatorFrontendAction_508::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("varDecl")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;

      if (const auto *InitExpr = VD->getInit()) {
        if (const auto *UnaryOp = dyn_cast<UnaryOperator>(InitExpr)) {
          if (UnaryOp->getOpcode() == UO_Deref) {
            if (const auto *Literal = dyn_cast<IntegerLiteral>(UnaryOp->getSubExpr())) {
              //Perform mutation on the source code text by applying string replacement
              std::string replacementText;
              if (existingNonPointerVar) {
                replacementText = "*" + existingNonPointerVar->getNameAsString();
              } else {
                replacementText = "int b = 10;\n*" + "b";
              }
              //Replace the original AST node with the mutated one
              Rewrite.ReplaceText(CharSourceRange::getTokenRange(VD->getSourceRange()), 
                                  "const long long " + VD->getNameAsString() + " = " + replacementText + ";");
            }
          }
        }
      }
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("existingVar")) {
      if (VD->getType()->isIntegerType()) {
        existingNonPointerVar = VD;
      }
    }
}
  
void MutatorFrontendAction_508::MutatorASTConsumer_508::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher targetMatcher = varDecl(hasInitializer(unaryOperator(hasOperatorName("*"), 
        hasUnaryOperand(integerLiteral())))).bind("varDecl");
    DeclarationMatcher existingVarMatcher = varDecl(hasType(isInteger())).bind("existingVar");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(targetMatcher, &callback);
    matchFinder.addMatcher(existingVarMatcher, &callback);
    matchFinder.matchAST(Context);
}