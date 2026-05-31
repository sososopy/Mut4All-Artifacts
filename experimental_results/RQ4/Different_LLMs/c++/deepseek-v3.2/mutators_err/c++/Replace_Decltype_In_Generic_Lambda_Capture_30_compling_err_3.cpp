//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Decltype_In_Generic_Lambda_Capture_30
 */ 
class MutatorFrontendAction_30 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(30)

private:
    class MutatorASTConsumer_30 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_30(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Replace_Decltype_In_Generic_Lambda_Capture_30.h"

// ========================================================================================================
#define MUT30_OUTPUT 1

void MutatorFrontendAction_30::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *DT = Result.Nodes.getNodeAs<clang::DecltypeTypeLoc>("DecltypeLoc")) {
      //Filter nodes in header files
      if (!DT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto *DRE = Result.Nodes.getNodeAs<clang::DeclRefExpr>("CapturedVar");
      if (!DRE) return;
      
      auto *Lambda = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambda");
      if (!Lambda) return;
      
      auto *Param = Result.Nodes.getNodeAs<clang::ParmVarDecl>("AutoParam");
      if (!Param) return;
      
      //Check if the decltype expression refers to a captured variable
      auto *VD = llvm::dyn_cast<clang::VarDecl>(DRE->getDecl());
      if (!VD) return;
      
      //Verify the variable is captured by the lambda
      bool isCaptured = false;
      for (auto &Capture : Lambda->captures()) {
        if (Capture.getCapturedVar() == VD) {
          isCaptured = true;
          break;
        }
      }
      if (!isCaptured) return;
      
      //Get source ranges
      auto DT_Range = CharSourceRange::getTokenRange(DT->getSourceRange());
      auto ParamName = Param->getName();
      if (ParamName.empty()) return;
      
      //Perform mutation on the source code text by applying string replacement
      std::string NewText = "decltype(" + ParamName.str() + ")";
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(DT_Range, NewText);
    }
}
  
void MutatorFrontendAction_30::MutatorASTConsumer_30::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = decltypeTypeLoc(
      hasAncestor(lambdaExpr().bind("Lambda")),
      hasDescendant(declRefExpr(
        to(varDecl(hasAncestor(lambdaExpr()))).bind("CapturedVar")
      ))
    ).bind("DecltypeLoc");
    
    auto ParamMatcher = parmVarDecl(
      hasType(autoType()),
      hasAncestor(lambdaExpr().bind("Lambda"))
    ).bind("AutoParam");
    
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(ParamMatcher, &callback);
    matchFinder.matchAST(Context);
}