//header file
#pragma once
#include "Mutator_base.h"

/**
 * TemplateInstiationOrderSwap_94
 */ 
class MutatorFrontendAction_TemplateInstiationOrderSwap_94 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(TemplateInstiationOrderSwap_94)

private:
    class MutatorASTConsumer_TemplateInstiationOrderSwap_94 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_TemplateInstiationOrderSwap_94(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
        std::vector<const clang::ClassTemplateSpecializationDecl*> getTemplateInstantiations() { return templateInstantiations; }
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
        std::vector<const clang::ClassTemplateSpecializationDecl*> templateInstantiations;
    };
};

//source file
#include "../include/Mutator_TemplateInstiationOrderSwap_94.h"

// ========================================================================================================
#define MUT94_OUTPUT 1

void MutatorFrontendAction_TemplateInstiationOrderSwap_94::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("TemplateInstantiations")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Record the node information to be used in the mutation process
      templateInstantiations.push_back(MT);
    }
}

void MutatorFrontendAction_TemplateInstiationOrderSwap_94::MutatorASTConsumer_TemplateInstiationOrderSwap_94::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateSpecializationDecl().bind("TemplateInstantiations");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
    // Perform mutation on the source code text by applying string replacement
    if (callback.getTemplateInstantiations().size() >= 2) {
        auto firstInstantiation = callback.getTemplateInstantiations()[0];
        auto secondInstantiation = callback.getTemplateInstantiations()[1];
        std::string firstInstantiationText = TheRewriter.getSourceMgr().getBufferData(
            TheRewriter.getSourceMgr().getFileID(firstInstantiation->getLocation()), false).str();
        std::string secondInstantiationText = TheRewriter.getSourceMgr().getBufferData(
            TheRewriter.getSourceMgr().getFileID(secondInstantiation->getLocation()), false).str();
        std::string mutatedText = secondInstantiationText + "\n" + firstInstantiationText;
        TheRewriter.ReplaceText(firstInstantiation->getSourceRange(), mutatedText);
    }
}