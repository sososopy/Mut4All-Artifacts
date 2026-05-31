//header file
#pragma once
#include "Mutator_base.h"

/**
 * Mutate_Defaulted_Template_Arg_With_Lambda_112
 */ 
class MutatorFrontendAction_112 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(112)

private:
    class MutatorASTConsumer_112 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_112(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::VarDecl *> suitableVariables;
    };
};

//source file
#include "../include/Mutate_Defaulted_Template_Arg_With_Lambda_112.h"

// ========================================================================================================
#define MUT112_OUTPUT 1

void MutatorFrontendAction_112::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("varDecl")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      //Record suitable variables for capture
      if (VD->getType()->isFunctionPointerType() || VD->getType()->isFunctionType() ||
          VD->getType()->isPointerType() || VD->getType()->isReferenceType() ||
          VD->getType()->isIntegralOrEnumerationType() || VD->getType()->isBooleanType()) {
        suitableVariables.push_back(VD);
      }
    }
    else if (auto *TP = Result.Nodes.getNodeAs<clang::TemplateTypeParmDecl>("templateParm")) {
      //Filter nodes in header files
      if (!TP || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TP->getLocation()))
        return;
      //Get the source code text of target node
      if (!TP->hasDefaultArgument())
        return;
      
      auto defaultArg = TP->getDefaultArgument();
      if (!defaultArg)
        return;
      
      //Check if default argument is a lambda expression
      bool isLambda = false;
      if (auto *Expr = dyn_cast<Expr>(defaultArg)) {
        if (Expr->getStmtClass() == Stmt::LambdaExprClass) {
          isLambda = true;
        }
      }
      if (!isLambda)
        return;
      
      //Perform mutation on the source code text by applying string replacement
      std::string newDefaultArg;
      if (!suitableVariables.empty()) {
        //Pick a random suitable variable
        size_t idx = getrandom::getRandomIndex(suitableVariables.size() - 1);
        auto var = suitableVariables[idx];
        std::string varName = var->getNameAsString();
        std::string varType = var->getType().getAsString();
        
        //Construct new lambda capturing the variable
        newDefaultArg = "[" + varName + "](auto arg) { return " + varName + "(arg); }";
      } else {
        //No suitable variable found, introduce dummy variable
        std::string dummyDecl = "auto dummy = false;\n";
        SourceLocation insertLoc = TP->getBeginLoc();
        Rewrite.InsertTextBefore(insertLoc, dummyDecl);
        newDefaultArg = "[dummy](auto arg) { return dummy; }";
      }
      
      //Replace the original AST node with the mutated one
      SourceRange defaultArgRange = TP->getDefaultArgLoc();
      if (defaultArgRange.isValid()) {
        Rewrite.ReplaceText(defaultArgRange, newDefaultArg);
      }
    }
}
  
void MutatorFrontendAction_112::MutatorASTConsumer_112::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher varMatcher = varDecl().bind("varDecl");
    DeclarationMatcher templateMatcher = templateTypeParmDecl(hasDefaultArgument(anything())).bind("templateParm");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(varMatcher, &callback);
    matchFinder.addMatcher(templateMatcher, &callback);
    matchFinder.matchAST(Context);
}