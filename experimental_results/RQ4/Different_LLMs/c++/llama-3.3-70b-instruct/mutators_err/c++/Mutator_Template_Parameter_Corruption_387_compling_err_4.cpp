//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Parameter_Corruption_387
 */ 
class MutatorFrontendAction_387 : public MutatorFrontendAction {
public:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef InFile) override {
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        return std::make_unique<MutatorASTConsumer_387>(TheRewriter);
    }

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
        virtual void run(const MatchFinder::MatchResult &Result) override;
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
    if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateDecl>("TemplateDecl")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocStart()))
        return;
      //Get the source code text of target node
      auto templateParams = MT->getTemplateParameters();
      for (auto I = templateParams->begin(); I != templateParams->end(); ++I) {
        if (auto param = *I) {
          if (auto typeParam = dyn_cast<TemplateTypeParmType>(param->getType())) {
            //Perform mutation on the source code text by applying string replacement
            std::string replacementType;
            if (!types.empty()) {
              replacementType = types[0]->getAsString();
              std::string declaration = Lexer::getSourceText(CharSourceRange::getTokenRange(MT->getSourceRange()), *Result.SourceManager, Result.Context->getLangOpts());
              declaration.replace(declaration.find(typeParam->getNameAsString()), typeParam->getNameAsString().size(), replacementType);
              //Replace the original AST node with the mutated one
              Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
            }
          }
        }
      }
    } else if (auto *DT = Result.Nodes.getNodeAs<clang::Type>("Type")) {
      //Filter nodes in header files
      if (!DT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DT->getLocStart()))
        return;
      types.push_back(DT);
    }
}
  
void MutatorFrontendAction_387::MutatorASTConsumer_387::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = templateDecl(isDefinition()).bind("TemplateDecl");
    TypeMatcher typeMatcher = type().bind("Type");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(typeMatcher, &callback);
    matchFinder.matchAST(Context);
}