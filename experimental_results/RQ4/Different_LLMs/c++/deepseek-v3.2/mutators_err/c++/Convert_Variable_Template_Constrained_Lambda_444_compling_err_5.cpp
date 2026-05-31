//header file
#pragma once
#include "Mutator_base.h"

/**
 * Convert_Variable_Template_Constrained_Lambda_444
 */ 
class MutatorFrontendAction_444 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(444)

private:
    class MutatorASTConsumer_444 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_444(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchCallback &Result);
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
        std::string helperName;
    };
};

//source file
#include "../include/Convert_Variable_Template_Constrained_Lambda_444.h"

// ========================================================================================================
#define MUT444_OUTPUT 1

void MutatorFrontendAction_444::Callback::run(const MatchFinder::MatchCallback &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("VarTemplate")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      //Check if it's a variable template with lambda initializer
      if (!VD->isTemplateDecl() || !VD->getInit())
        return;
      auto *InitExpr = VD->getInit();
      if (!llvm::isa<clang::LambdaExpr>(InitExpr))
        return;
      auto *Lambda = llvm::cast<clang::LambdaExpr>(InitExpr);
      //Check if lambda has a requires-clause directly attached
      if (!Lambda->getTrailingRequiresClause())
        return;
      //Get the source code text of target node
      auto VarRange = VD->getSourceRange();
      auto VarText = stringutils::rangetoStr(*(Result.SourceManager), VarRange);
      //Generate helper struct name
      helperName = VD->getNameAsString() + "_mut";
      //Extract constraint expression from lambda
      auto ConstraintRange = Lambda->getTrailingRequiresClause()->getSourceRange();
      auto ConstraintText = stringutils::rangetoStr(*(Result.SourceManager), ConstraintRange);
      //Get template parameters
      auto *TemplateDecl = VD->getDescribedVarTemplate();
      auto *TemplateParams = TemplateDecl->getTemplateParameters();
      std::string TemplateParamText;
      if (TemplateParams) {
        TemplateParamText = stringutils::rangetoStr(*(Result.SourceManager), TemplateParams->getSourceRange());
      }
      //Perform mutation on the source code text by applying string replacement
      std::string HelperStruct = "\ntemplate" + TemplateParamText + "\nstruct " + helperName + " {\n    static void func() requires " + ConstraintText + ";\n};";
      std::string NewVarDecl = "\ntemplate" + TemplateParamText + "\nauto " + VD->getNameAsString() + " = []() requires requires { " + helperName + "<T>::func(); } { };";
      std::string MutatedText = HelperStruct + "\n" + NewVarDecl;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(VarRange), MutatedText);
    }
}
  
void MutatorFrontendAction_444::MutatorASTConsumer_444::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(isTemplateDecl(), hasInitializer(lambdaExpr())).bind("VarTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}