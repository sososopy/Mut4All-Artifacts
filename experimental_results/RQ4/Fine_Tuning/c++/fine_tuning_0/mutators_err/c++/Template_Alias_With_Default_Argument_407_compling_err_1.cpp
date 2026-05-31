//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Alias_With_Default_Argument_407
 */ 
class MutatorFrontendAction_407 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(407)

private:
    class MutatorASTConsumer_407 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_407(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Template_Alias_With_Default_Argument_407.h"

// ========================================================================================================
#define MUT407_OUTPUT 1

void MutatorFrontendAction_407::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TypeAliasDecl>("TypeAlias")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto alias_name = MT->getNameAsString();
      llvm::outs() << alias_name << '\n';
      auto TPL = MT->getTemplateParameters();
      if (TPL == nullptr)
        return;
      auto TP = TPL->asArray();
      if (TP.size() == 0)
        return;
      for (auto param : TP) {
        if (param->isTemplateParameter() &&
            param->isTemplateParameterPack() == false) {
          auto PT = dyn_cast<TemplateTypeParmDecl>(param);
          if (PT->hasDefaultArgument()) {
            auto DA = PT->getDefaultArgument();
            if (DA->isInstantiationDependent() &&
                DA->isDependentType() == false) {
              //Perform mutation on the source code text by applying string replacement
              auto alias_decl = stringutils::rangetoStr(
                  *(Result.SourceManager), MT->getSourceRange());
              alias_decl = "/*mut407*/" + alias_decl;
              //Replace the original AST node with the mutated one
              Rewrite.ReplaceText(MT->getBeginLoc(), 0, alias_decl + ";\n");
              llvm::outs() << alias_decl << '\n';
              auto alias_inst = alias_name + "<>";
              Rewrite.ReplaceText(MT->getEndLoc(), 0,
                                  ";\nvoid mut407(" + alias_inst + "){}");
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_407::MutatorASTConsumer_407::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = typeAliasDecl().bind("TypeAlias");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}