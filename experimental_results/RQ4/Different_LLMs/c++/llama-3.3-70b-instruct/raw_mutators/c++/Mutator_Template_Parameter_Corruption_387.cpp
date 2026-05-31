```cpp
//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Parameter_Corruption_387
 */ 
class MutatorFrontendAction_387 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(387)

private:
    class MutatorASTConsumer_387 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_387(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::Type *> types;
    };
};

//source file
#include "../include/Mutator_Template_Parameter_Corruption_387.h"

// ========================================================================================================
#define MUT387_OUTPUT 1

void MutatorFrontendAction_387::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateDecl>(("TemplateDecl"))) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto templateParams = MT->getTemplateParameters();
      for (auto param : templateParams) {
        if (auto typeParam = dyn_cast<TemplateTypeParmType>(param->getType())) {
          //Perform mutation on the source code text by applying string replacement
          std::string replacementType;
          if (!types.empty()) {
            replacementType = types[getrandom::getRandomIndex(types.size())]->getAsString();
            std::string declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                           MT->getSourceRange());
            declaration.replace(declaration.find(typeParam->getNameAsString()), typeParam->getNameAsString().size(), replacementType);
            //Replace the original AST node with the mutated one
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
          }
        }
      }
    } else if (auto *DT = Result.Nodes.getNodeAs<clang::Type>(("Type"))) {
      //Filter nodes in header files
      if (!DT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DT->getLocation()))
        return;
      types.push_back(DT);
    }
}
  
void MutatorFrontendAction_387::MutatorASTConsumer_387::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = templateDecl().bind("TemplateDecl");
    DeclarationMatcher typeMatcher = type().bind("Type");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(typeMatcher, &callback);
    matchFinder.matchAST(Context);
}