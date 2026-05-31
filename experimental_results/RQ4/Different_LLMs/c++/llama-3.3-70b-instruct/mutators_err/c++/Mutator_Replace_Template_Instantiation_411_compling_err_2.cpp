//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_Instantiation_411
 */ 
class MutatorFrontendAction_411 : public MutatorFrontendAction {
public:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef InFile) override {
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        return std::make_unique<MutatorASTConsumer_411>(TheRewriter);
    }

private:
    class MutatorASTConsumer_411 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_411(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/Mutator_Replace_Template_Instantiation_411.h"

// ========================================================================================================
#define MUT411_OUTPUT 1

void MutatorFrontendAction_411::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateSpecializationType>("TemplateInstantiation")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocStart()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangeToString(*(Result.SourceManager),
                                                 MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::string newType = "int";  // Replace with a new type, e.g., int
      size_t pos = declaration.find(MT->getTemplateName().getAsTemplateDecl()->getNameAsString());
      if (pos != std::string::npos) {
        declaration.replace(pos, MT->getTemplateName().getAsTemplateDecl()->getNameAsString().length(), newType);
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_411::MutatorASTConsumer_411::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    TypeMatcher matcher = templateSpecializationType().bind("TemplateInstantiation");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}