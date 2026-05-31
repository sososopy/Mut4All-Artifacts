//header file
#pragma once
#include "Mutator_base.h"

/**
 * TemplateInstantiationSwapper_110
 */ 
class MutatorFrontendAction_TemplateInstantiationSwapper_110 : public MutatorFrontendAction {
public:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef File) override {
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        return std::make_unique<MutatorASTConsumer_TemplateInstantiationSwapper_110>(TheRewriter);
    }

private:
    class MutatorASTConsumer_TemplateInstantiationSwapper_110 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_TemplateInstantiationSwapper_110(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::TemplateSpecializationTypeLoc*> templateInstantiations;
    };
};

//source file
#include "../include/Mutator_TemplateInstantiationSwapper_110.h"

// ========================================================================================================
#define MUT110_OUTPUT 1

void MutatorFrontendAction_TemplateInstantiationSwapper_110::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TST = Result.Nodes.getNodeAs<clang::TemplateSpecializationTypeLoc>("TemplateInstantiation")) {
      //Filter nodes in header files
      if (!TST || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TST->getBeginLoc()))
        return;
      //Record the template instantiation
      templateInstantiations.push_back(TST);
      if (templateInstantiations.size() >= 2) {
        //Get the source code text of target node
        auto loc1 = templateInstantiations[templateInstantiations.size() - 2]->getBeginLoc();
        auto loc2 = templateInstantiations[templateInstantiations.size() - 1]->getBeginLoc();
        auto range1 = CharSourceRange::getTokenRange(loc1, loc2);
        auto range2 = CharSourceRange::getTokenRange(loc2, loc1);
        //Perform mutation on the source code text by applying string replacement
        std::string text1 = Lexer::getSourceText(range1, *Result.SourceManager, LangOptions()).str();
        std::string text2 = Lexer::getSourceText(range2, *Result.SourceManager, LangOptions()).str();
        //Swap the template arguments
        std::string mutatedText1 = text2;
        std::string mutatedText2 = text1;
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(range1, mutatedText1);
        Rewrite.ReplaceText(range2, mutatedText2);
      }
    }
}
  
void MutatorFrontendAction_TemplateInstantiationSwapper_110::MutatorASTConsumer_TemplateInstantiationSwapper_110::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    TypeLocMatcher matcher = templateSpecializationTypeLoc().bind("TemplateInstantiation");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}