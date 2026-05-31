//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Constrained_Template_Function_Return_Type_Deduction_With_Explicit_Return_Type_Using_Decltype_On_Constraint_Expression_395
 */ 
class MutatorFrontendAction_395 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(395)

private:
    class MutatorASTConsumer_395 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_395(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Replace_Constrained_Template_Function_Return_Type_Deduction_With_Explicit_Return_Type_Using_Decltype_On_Constraint_Expression_395.h"

// ========================================================================================================
#define MUT395_OUTPUT 1

void MutatorFrontendAction_395::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("constrainedAutoFunction")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Get the source code text of target node
      if (!FD->isTemplate() || !FD->getReturnType()->isAutoType())
        return;
      auto *TemplateDecl = FD->getDescribedFunctionTemplate();
      if (!TemplateDecl)
        return;
      const auto *RequiresClause = TemplateDecl->getTrailingRequiresClause();
      if (!RequiresClause)
        return;
      //Find decltype expression in requires clause
      std::string DecltypeExprStr;
      auto FindDecltypeExpr = [&](const clang::Expr *E) {
        if (const auto *DRE = dyn_cast<clang::DeclRefExpr>(E)) {
          if (DRE->getDecl()->getDeclName().isIdentifier()) {
            DecltypeExprStr = DRE->getDecl()->getNameAsString();
          }
        } else if (const auto *CE = dyn_cast<clang::CastExpr>(E)) {
          if (const auto *SubExpr = CE->getSubExpr()) {
            FindDecltypeExpr(SubExpr);
          }
        } else if (const auto *UE = dyn_cast<clang::UnaryExpr>(E)) {
          if (const auto *SubExpr = UE->getSubExpr()) {
            FindDecltypeExpr(SubExpr);
          }
        } else if (const auto *BE = dyn_cast<clang::BinaryExpr>(E)) {
          FindDecltypeExpr(BE->getLHS());
          FindDecltypeExpr(BE->getRHS());
        } else if (const auto *CExpr = dyn_cast<clang::CallExpr>(E)) {
          for (const auto *Arg : CExpr->getArgs()) {
            FindDecltypeExpr(Arg);
          }
        }
      };
      FindDecltypeExpr(RequiresClause);
      if (DecltypeExprStr.empty())
        return;
      //Perform mutation on the source code text by applying string replacement
      std::string FunctionText = stringutils::rangetoStr(*Result.SourceManager, FD->getSourceRange());
      size_t AutoPos = FunctionText.find("auto");
      if (AutoPos != std::string::npos) {
        FunctionText.replace(AutoPos, 4, "decltype(" + DecltypeExprStr + ")");
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), FunctionText);
    }
}
  
void MutatorFrontendAction_395::MutatorASTConsumer_395::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(isTemplate(), hasReturnType(autoType()), hasTrailingRequiresClause()).bind("constrainedAutoFunction");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}