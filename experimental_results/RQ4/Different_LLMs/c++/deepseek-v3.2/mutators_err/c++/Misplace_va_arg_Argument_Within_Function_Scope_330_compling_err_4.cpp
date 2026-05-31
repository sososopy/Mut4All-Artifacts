//header file
#pragma once
#include "Mutator_base.h"

/**
 * Misplace_va_arg_Argument_Within_Function_Scope_330
 */ 
class MutatorFrontendAction_330 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(330)

private:
    class MutatorASTConsumer_330 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_330(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Misplace_va_arg_Argument_Within_Function_Scope_330.h"

// ========================================================================================================
#define MUT330_OUTPUT 1

void MutatorFrontendAction_330::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CallExpr>("vaArgCall")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;

      // Check if it's a __builtin_va_arg call
      const FunctionDecl *FD = MT->getDirectCallee();
      if (!FD || FD->getNameAsString() != "__builtin_va_arg")
        return;

      // Get the first argument (va_list variable)
      if (MT->getNumArgs() < 1)
        return;
      const Expr *vaArgExpr = MT->getArg(0);
      if (!vaArgExpr)
        return;

      // Find the enclosing function
      const DeclContext *DC = Result.Context->getParents(*MT)[0].get<DeclContext>();
      if (!DC || !isa<FunctionDecl>(DC))
        return;
      const FunctionDecl *EnclosingFunc = cast<FunctionDecl>(DC);

      // Collect all non-va_list variables in the function scope
      std::vector<const VarDecl*> candidateVars;
      for (const Decl *D : EnclosingFunc->decls()) {
        if (const VarDecl *VD = dyn_cast<VarDecl>(D)) {
          // Check if variable is not of va_list type
          QualType T = VD->getType();
          if (!T->isVariablyModifiedType() && !T.getTypePtr()->isVoidType()) {
            candidateVars.push_back(VD);
          }
        }
      }

      // Also consider parameters
      for (unsigned i = 0; i < EnclosingFunc->getNumParams(); ++i) {
        const ParmVarDecl *PD = EnclosingFunc->getParamDecl(i);
        QualType T = PD->getType();
        if (!T->isVariablyModifiedType() && !T.getTypePtr()->isVoidType()) {
          candidateVars.push_back(PD);
        }
      }

      // If no suitable variable exists, introduce a new int variable before the call
      if (candidateVars.empty()) {
        // Insert a new int variable declaration just before the __builtin_va_arg call
        SourceLocation InsertLoc = MT->getBeginLoc();
        std::string newVarDecl = "int __mut330_var = 0; ";
        Rewrite.InsertTextBefore(InsertLoc, newVarDecl);
        // Replace the va_list argument with the new variable
        Rewrite.ReplaceText(vaArgExpr->getSourceRange(), "__mut330_var");
      } else {
        // Choose a random candidate variable
        size_t idx = getRandomIndex(candidateVars.size() - 1);
        const VarDecl *replacementVar = candidateVars[idx];
        // Replace the va_list argument with the chosen variable
        Rewrite.ReplaceText(vaArgExpr->getSourceRange(), replacementVar->getNameAsString());
      }
    }
}
  
void MutatorFrontendAction_330::MutatorASTConsumer_330::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = callExpr(callee(functionDecl(hasName("__builtin_va_arg")))).bind("vaArgCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}