//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Int_Const_With_Template_Param_185
 */ 
class MutatorFrontendAction_185 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(185)

private:
    class MutatorASTConsumer_185 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_185(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::TemplateTypeParmDecl *> templateParams;
    };
};

//source file
#include "../include/Mutator_Replace_Int_Const_With_Template_Param_185.h"

// ========================================================================================================
#define MUT185_OUTPUT 1

void MutatorFrontendAction_185::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TP = Result.Nodes.getNodeAs<clang::TemplateTypeParmDecl>("TemplateParam")) {
      //Filter nodes in header files
      if (!TP || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TP->getLocation()))
        return;
      //Record the node information to be used in the mutation process
      templateParams.push_back(TP);
    }
    else if (auto *BC = Result.Nodes.getNodeAs<clang::BinaryOperator>("BinaryComp")) {
      //Filter nodes in header files
      if (!BC || !Result.Context->getSourceManager().isWrittenInMainFile(
                     BC->getLocation()))
        return;
      if (!BC->isComparisonOp())
        return;
      //Check if one operand is an integer constant
      clang::Expr *LHS = BC->getLHS()->IgnoreParenImpCasts();
      clang::Expr *RHS = BC->getRHS()->IgnoreParenImpCasts();
      clang::IntegerLiteral *IntLit = nullptr;
      bool LHSIsInt = false;
      if (clang::IntegerLiteral *Lit = clang::dyn_cast<clang::IntegerLiteral>(LHS)) {
        IntLit = Lit;
        LHSIsInt = true;
      } else if (clang::IntegerLiteral *Lit = clang::dyn_cast<clang::IntegerLiteral>(RHS)) {
        IntLit = Lit;
        LHSIsInt = false;
      }
      if (!IntLit)
        return;
      //Ensure the binary operator is inside a template function
      clang::DeclContext *DC = Result.Context->getParents(*BC)[0].get<clang::Decl>();
      while (DC && !clang::isa<clang::FunctionDecl>(DC))
        DC = DC->getParent();
      if (!DC)
        return;
      clang::FunctionDecl *FD = clang::dyn_cast<clang::FunctionDecl>(DC);
      if (!FD || !FD->getTemplatedKind())
        return;
      //Get the source code text of target node
      std::string IntText = stringutils::rangetoStr(*(Result.SourceManager), IntLit->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      if (templateParams.empty())
        return;
      size_t idx = getrandom::getRandomIndex(templateParams.size() - 1);
      std::string ParamName = templateParams[idx]->getNameAsString();
      if (ParamName.empty())
        return;
      std::string Replacement = ParamName;
      //Replace the original AST node with the mutated one
      if (LHSIsInt) {
        Rewrite.ReplaceText(IntLit->getSourceRange(), Replacement);
      } else {
        Rewrite.ReplaceText(IntLit->getSourceRange(), Replacement);
      }
    }
}
  
void MutatorFrontendAction_185::MutatorASTConsumer_185::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher paramMatcher = templateTypeParmDecl().bind("TemplateParam");
    StatementMatcher compMatcher = binaryOperator(hasOperatorName("==")
        .or(hasOperatorName("!="))
        .or(hasOperatorName("<"))
        .or(hasOperatorName(">"))
        .or(hasOperatorName("<="))
        .or(hasOperatorName(">="))).bind("BinaryComp");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(paramMatcher, &callback);
    matchFinder.addMatcher(compMatcher, &callback);
    matchFinder.matchAST(Context);
}