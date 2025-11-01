//header file
#pragma once
#include "Mutator_base.h"

/**
 * Conflicting_Template_Specialization_147
 */ 
class MutatorFrontendAction_147 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(147)

private:
    class MutatorASTConsumer_147 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_147(Rewriter &R) : TheRewriter(R) {}
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
        std::set<std::string> specializedTemplates;
    };
};

//source file
#include "../include/conflicting_template_specialization_147.h"

// ========================================================================================================
#define MUT147_OUTPUT 1

void MutatorFrontendAction_147::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("ClassTemplate")) {
        if (!CT || !Result.Context->getSourceManager().isWrittenInMainFile(CT->getLocation()))
            return;

        std::string templateName = CT->getNameAsString();
        if (specializedTemplates.find(templateName) != specializedTemplates.end())
            return;

        specializedTemplates.insert(templateName);
        std::string specializationCode = "template<typename T> class " + templateName + "<T*> {};";

        SourceLocation insertLoc = CT->getEndLoc().getLocWithOffset(1);
        Rewrite.InsertText(insertLoc, "\n/*mut147*/" + specializationCode);
    }
}

void MutatorFrontendAction_147::MutatorASTConsumer_147::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl().bind("ClassTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}