//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Specialization_150
 */ 
class MutatorFrontendAction_150 : public MutatorFrontendAction {
public:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef File) override {
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        return std::make_unique<MutatorASTConsumer_150>(TheRewriter);
    }

private:
    class MutatorASTConsumer_150 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_150(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::ClassTemplateSpecializationDecl *> templateSpecializations;
    };
    Rewriter TheRewriter;
    static std::string getNewType(const clang::ClassTemplateSpecializationDecl *TS);
    static std::string replaceType(const std::string &declaration, const std::string &newType);
};

