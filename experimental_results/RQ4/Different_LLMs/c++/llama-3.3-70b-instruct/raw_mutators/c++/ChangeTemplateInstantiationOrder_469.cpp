//header file
#pragma once
#include "Mutator_base.h"

/**
 * ChangeTemplateInstantiationOrder_469
 */ 
class MutatorFrontendAction_469 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(469)

private:
    class MutatorASTConsumer_469 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_469(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::ClassTemplateDecl *> templateDecls;
    };
};

//source file
#include "../include/ChangeTemplateInstantiationOrder_469.h"

// ========================================================================================================
#define MUT469_OUTPUT 1

void MutatorFrontendAction_469::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateDecl")) {
      //Filter nodes in header files
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;
      //Record the template declaration
      templateDecls.push_back(TD);
    } else if (auto *TI = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("TemplateInstantiation")) {
      //Filter nodes in header files
      if (!TI || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TI->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 TI->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      // Swap the order of template parameters
      auto templateParams = TI->getTemplateArguments();
      std::vector<clang::TemplateArgument> newParams;
      for (int i = templateParams.size() - 1; i >= 0; --i) {
        newParams.push_back(templateParams[i]);
      }
      auto newDeclaration = declaration;
      for (int i = 0; i < templateParams.size(); ++i) {
        auto paramStr = stringutils::rangetoStr(*(Result.SourceManager),
                                                 templateParams[i].getSourceRange());
        auto newParamStr = stringutils::rangetoStr(*(Result.SourceManager),
                                                     newParams[i].getSourceRange());
        newDeclaration.replace(newDeclaration.find(paramStr), paramStr.size(), newParamStr);
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(TI->getSourceRange()), newDeclaration);
    }
}
  
void MutatorFrontendAction_469::MutatorASTConsumer_469::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher1 = classTemplateDecl().bind("TemplateDecl");
    DeclarationMatcher matcher2 = classTemplateSpecializationDecl().bind("TemplateInstantiation");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher1, &callback);
    matchFinder.addMatcher(matcher2, &callback);
    matchFinder.matchAST(Context);
}