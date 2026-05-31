//header file
#pragma once
#include "Mutator_base.h"

/**
 * TemplateInstantiation_Mutator_260
 */ 
class MutatorFrontendAction_TemplateInstantiation_260 : public MutatorFrontendAction {
public:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef File) override {
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        return std::make_unique<MutatorASTConsumer_TemplateInstantiation_260>(TheRewriter);
    }

private:
    class MutatorASTConsumer_TemplateInstantiation_260 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_TemplateInstantiation_260(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_TemplateInstantiation_260.h"

// ========================================================================================================
#define TEMPLATEINSTANTIATION_OUTPUT 1

void MutatorFrontendAction_TemplateInstantiation_260::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("TemplateSpecializations")) {
      //Filter nodes in header files
      if (!MT ||!Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                               MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      // Replace template parameter with a different type
      std::string newType = "int"; // Replace with a different type
      size_t pos = declaration.find("typename T");
      if (pos!= std::string::npos) {
        declaration.replace(pos, 11, newType);
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_TemplateInstantiation_260::MutatorASTConsumer_TemplateInstantiation_260::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateSpecializationDecl().bind("TemplateSpecializations");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}