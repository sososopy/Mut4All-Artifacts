//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Non_Dependent_Expression_In_Template_With_Dependent_136
 */ 
class MutatorFrontendAction_136 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(136)

private:
    class MutatorASTConsumer_136 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_136(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::TemplateTypeParmDecl*> templateParams;
    };
};

//source file
#include "../include/Mutator_136.h"

// ========================================================================================================
#define MUT136_OUTPUT 1

void MutatorFrontendAction_136::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TP = Result.Nodes.getNodeAs<clang::TemplateTypeParmDecl>("TemplateParam")) {
      //Filter nodes in header files
      if (!TP || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TP->getLocation()))
        return;
      //Record the node information to be used in the mutation process
      templateParams.push_back(TP);
    }
    else if (auto *MT = Result.Nodes.getNodeAs<clang::IntegerLiteral>("NonDepExpr")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto exprText = stringutils::rangetoStr(*(Result.SourceManager), MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      if (templateParams.empty()) return;
      size_t index = getrandom::getRandomIndex(templateParams.size() - 1);
      auto param = templateParams[index];
      std::string paramName = param->getNameAsString();
      if (paramName.empty()) paramName = "T";
      std::string newExpr = "sizeof(" + paramName + ")";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), newExpr);
    }
}
  
void MutatorFrontendAction_136::MutatorASTConsumer_136::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher paramMatcher = templateTypeParmDecl().bind("TemplateParam");
    StatementMatcher exprMatcher = integerLiteral(unless(isInTemplateInstantiation())).bind("NonDepExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(paramMatcher, &callback);
    matchFinder.addMatcher(exprMatcher, &callback);
    matchFinder.matchAST(Context);
}